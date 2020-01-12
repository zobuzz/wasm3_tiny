//
//  ViewController.h
//  wams3test
//
//  Created by zorro on 2020/1/7.
//  Copyright © 2020 zorro. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController


@end

@interface View : UIView
{
    CADisplayLink* m_displayLink;
}

@property(null_resettable, nonatomic,strong) ViewController* viewController; // The getter first invokes [self loadView] if the view hasn't been set yet. Subclasses must call super if they override the setter or getter.

//- (void) set_bgfx_platform_data;

- (void)start;

@end
