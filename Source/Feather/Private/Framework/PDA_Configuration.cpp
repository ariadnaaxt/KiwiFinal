#include "Framework/PDA_Configuration.h"

TArray<FDialogConfiguration> UPDA_Configuration::GetDialogFromId(const FName& DialogId) const
{
	TArray<FDialogConfiguration> DialogConfig;

	if (Dialogs.Contains(DialogId) == true)
	{
		const FDialogConfigWrapper DialogConfigWrapper = Dialogs.FindRef(DialogId);
		DialogConfig = DialogConfigWrapper.Dialogs;
	}

	return DialogConfig;
}
