////////////////////////////////////////////////
//  2023 Prùcha Filip <prucha.fil@gmail.com>  //
////////////////////////////////////////////////

#include "pch.h"
#include "application.h"
#include "tab.h"

wxIMPLEMENT_APP(application);

bool application::OnInit() {
    tab* t = new tab();
    return t->Show();

}