/*************************************************************************/
/*  view_controller.mm                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#import "view_controller.h"
#include "core/config/project_settings.h"
#include "display_server_iphone.h"
#import "Fox_view.h"
#import "Fox_view_renderer.h"
#import "keyboard_input_view.h"
#include "os_iphone.h"

#import <AVFoundation/AVFoundation.h>
#import <GameController/GameController.h>

@interface ViewController () <FoxViewDelegate>

@property(strong, nonatomic) FoxViewRenderer *renderer;
@property(strong, nonatomic) FoxKeyboardInputView *keyboardView;

@property(strong, nonatomic) UIView *FoxLoadingOverlay;

@end

@implementation ViewController

- (FoxView *)FoxView {
	return (FoxView *)self.view;
}

- (void)loadView {
	FoxView *view = [[FoxView alloc] init];
	FoxViewRenderer *renderer = [[FoxViewRenderer alloc] init];

	self.renderer = renderer;
	self.view = view;

	view.renderer = self.renderer;
	view.delegate = self;
}

- (instancetype)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
	self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];

	if (self) {
		[self Fox_commonInit];
	}

	return self;
}

- (instancetype)initWithCoder:(NSCoder *)coder {
	self = [super initWithCoder:coder];

	if (self) {
		[self Fox_commonInit];
	}

	return self;
}

- (void)Fox_commonInit {
	// Initialize view controller values.
}

- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
	printf("*********** did receive memory warning!\n");
}

- (void)viewDidLoad {
	[super viewDidLoad];

	[self observeKeyboard];
	[self displayLoadingOverlay];

	if (@available(iOS 11.0, *)) {
		[self setNeedsUpdateOfScreenEdgesDeferringSystemGestures];
	}
}

- (void)observeKeyboard {
	printf("******** setting up keyboard input view\n");
	self.keyboardView = [FoxKeyboardInputView new];
	[self.view addSubview:self.keyboardView];

	printf("******** adding observer for keyboard show/hide\n");
	[[NSNotificationCenter defaultCenter]
			addObserver:self
			   selector:@selector(keyboardOnScreen:)
				   name:UIKeyboardDidShowNotification
				 object:nil];
	[[NSNotificationCenter defaultCenter]
			addObserver:self
			   selector:@selector(keyboardHidden:)
				   name:UIKeyboardDidHideNotification
				 object:nil];
}

- (void)displayLoadingOverlay {
	NSBundle *bundle = [NSBundle mainBundle];
	NSString *storyboardName = @"Launch Screen";

	if ([bundle pathForResource:storyboardName ofType:@"storyboardc"] == nil) {
		return;
	}

	UIStoryboard *launchStoryboard = [UIStoryboard storyboardWithName:storyboardName bundle:bundle];

	UIViewController *controller = [launchStoryboard instantiateInitialViewController];
	self.FoxLoadingOverlay = controller.view;
	self.FoxLoadingOverlay.frame = self.view.bounds;
	self.FoxLoadingOverlay.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

	[self.view addSubview:self.FoxLoadingOverlay];
}

- (BOOL)FoxViewFinishedSetup:(FoxView *)view {
	[self.FoxLoadingOverlay removeFromSuperview];
	self.FoxLoadingOverlay = nil;

	return YES;
}

- (void)dealloc {
	self.keyboardView = nil;

	self.renderer = nil;

	if (self.FoxLoadingOverlay) {
		[self.FoxLoadingOverlay removeFromSuperview];
		self.FoxLoadingOverlay = nil;
	}

	[[NSNotificationCenter defaultCenter] removeObserver:self];
}

// MARK: Orientation

- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures {
	return UIRectEdgeAll;
}

- (BOOL)shouldAutorotate {
	if (!DisplayServerIPhone::get_singleton()) {
		return NO;
	}

	switch (DisplayServerIPhone::get_singleton()->screen_get_orientation(DisplayServer::SCREEN_OF_MAIN_WINDOW)) {
		case DisplayServer::SCREEN_SENSOR:
		case DisplayServer::SCREEN_SENSOR_LANDSCAPE:
		case DisplayServer::SCREEN_SENSOR_PORTRAIT:
			return YES;
		default:
			return NO;
	}
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
	if (!DisplayServerIPhone::get_singleton()) {
		return UIInterfaceOrientationMaskAll;
	}

	switch (DisplayServerIPhone::get_singleton()->screen_get_orientation(DisplayServer::SCREEN_OF_MAIN_WINDOW)) {
		case DisplayServer::SCREEN_PORTRAIT:
			return UIInterfaceOrientationMaskPortrait;
		case DisplayServer::SCREEN_REVERSE_LANDSCAPE:
			return UIInterfaceOrientationMaskLandscapeRight;
		case DisplayServer::SCREEN_REVERSE_PORTRAIT:
			return UIInterfaceOrientationMaskPortraitUpsideDown;
		case DisplayServer::SCREEN_SENSOR_LANDSCAPE:
			return UIInterfaceOrientationMaskLandscape;
		case DisplayServer::SCREEN_SENSOR_PORTRAIT:
			return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
		case DisplayServer::SCREEN_SENSOR:
			return UIInterfaceOrientationMaskAll;
		case DisplayServer::SCREEN_LANDSCAPE:
			return UIInterfaceOrientationMaskLandscapeLeft;
	}
};

- (BOOL)prefersStatusBarHidden {
	return YES;
}

- (BOOL)prefersHomeIndicatorAutoHidden {
	if (GLOBAL_GET("display/window/ios/hide_home_indicator")) {
		return YES;
	} else {
		return NO;
	}
}

// MARK: Keyboard

- (void)keyboardOnScreen:(NSNotification *)notification {
	NSDictionary *info = notification.userInfo;
	NSValue *value = info[UIKeyboardFrameEndUserInfoKey];

	CGRect rawFrame = [value CGRectValue];
	CGRect keyboardFrame = [self.view convertRect:rawFrame fromView:nil];

	if (DisplayServerIPhone::get_singleton()) {
		DisplayServerIPhone::get_singleton()->virtual_keyboard_set_height(keyboardFrame.size.height);
	}
}

- (void)keyboardHidden:(NSNotification *)notification {
	if (DisplayServerIPhone::get_singleton()) {
		DisplayServerIPhone::get_singleton()->virtual_keyboard_set_height(0);
	}
}

@end
