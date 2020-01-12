//
//  AppDelegate.h
//  wams3test
//
//  Created by zorro on 2020/1/7.
//  Copyright © 2020 zorro. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "ViewController.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    UIWindow* window;
    View* m_view;
}

@property (nonatomic, retain) UIWindow* window;
@property (nonatomic, retain) View* m_view;

@end
