#import <UIKit/UIKit.h>

extern "C"
int frog_is_ipad(void)
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
       return 1;
    }
    return 0;
}
