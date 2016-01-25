#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xol/OpenLook.h>
#include <Xol/StaticText.h>

#define STRING "Hello, World"

Arg             _tmpArgs[] = {
  {XtNstring, (XtArgVal) STRING},
};


main(argc, argv)
  int             argc;
  char          **argv;
{
  Widget          w_top, w_text;

  w_top = OlInitialize(argv[0],
		       "HelloWorld",
		       NULL,
		       0,
		       &argc,
		       argv
    );

  w_text = XtCreateWidget("text",
				 staticTextWidgetClass,
				 w_top,
				 _tmpArgs,
				 XtNumber(_tmpArgs)
    );
  XtManageChild(w_text);

  XtRealizeWidget(w_top);
  XtMainLoop();
}
