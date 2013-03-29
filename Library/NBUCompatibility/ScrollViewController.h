//
//  ScrollViewController.h
//  NBUBase
//
//  Created by エルネスト 利辺羅 on 12/02/07.
//  Copyright (c) 2012年 CyberAgent Inc. All rights reserved.
//

#import "NBUViewController.h"

// Notification observed to trigger/cancel image load
extern NSString * const ScrollViewContentOffsetChangedNotification;
extern NSString * const ScrollViewDidScrollNotification;
extern NSString * const ScrollViewEndScrollNotification;

@protocol AnalyticsDelegate;

/**
 Base UIViewController with many convenience methods.

 - Manage UIScrollView contentSize on viewWillAppear.
 - Create a UIScrollView if necessary.
 - Automatically SizeToFit contentView.
 - Manage hiding the keyboard.
 - Autohide prompt messages.
 - Hide/show UINavigationBar and UITabBar on scroll.
 - Customize buck button titles.
 
 @note Can be initialized from a Nib file or programatically.
 */
@interface ScrollViewController : NBUViewController <UIScrollViewDelegate>

/// @name Outlets

/// Managed UIScrollView (may be the controller's view or a subview of that view).
@property (nonatomic, strong) IBOutlet  UIScrollView * scrollView;

/// Force scroll to top.
- (void)resetScrollViewOffset;

/// The view that is used to adjust the scrollview contentSize.
///
/// If not set it will be scrollview's first subview.
@property (nonatomic, strong) IBOutlet  UIView * contentView;

/// The AnalyticsDelegate.
@property (nonatomic, assign) IBOutlet id<AnalyticsDelegate> analyticsDelegate;

/// @name Configurable Properties

/// A boolean that indicates whether the controller should animate its contentView. Default `NO`.
@property (nonatomic, getter = isAnimated) BOOL animated;

/// The currently active field.
@property (nonatomic, strong, readonly) UIView * activeField;

/// Empty or nil string will force to use the system default back button.
@property (nonatomic, strong)           NSString * customBackButtonTitle;

/// A NSString to be used for analytics tracking
/// @see AnalyticsHelper
@property (nonatomic, strong)           NSString * analyticsTag;

/// Whether to hide or not navigation and tab bars on scroll. Default `NO`.
@property (nonatomic)                   BOOL hidesBarsOnScroll;

/// @name Managing the Prompt

/// Display a short message over the UINavigationBar
/// @param prompt The NSString to be shown.
/// @param yesOrNo Whether to clear the prompt automatically after 3 seconds
- (void)setPrompt:(NSString *)prompt
        autoClear:(BOOL)yesOrNo;

/// Force to clear the prompt immediatly
- (void)clearPrompt;

/// @name Methods/Actions

/// Set a custom back button title for all ScrollViewController and subclasses instances.
/// @param title The custom title to be used.
/// @note Can be overriden per instance by setting the customBackButtonTitle to an empty string.
+ (void)setCustomBackButtonTitle:(NSString *)title;

/// Notify listeners (usually ImageLoadingView subviews) that the scrollView offset has changed
- (void)postScrollViewContentOffsetChangedNotification;

- (void)postScrollVieEndNotification;

/// Adjust contentView's size and scrollview's contentsize.
/// @param sender The sender object.
- (IBAction)sizeToFitContentView:(id)sender;

/// Force to hide the keyboard.
/// @param sender The sender object.
/// @note In most cases the controller will hide it automatically
- (IBAction)hideKeyboard:(id)sender;

- (void)keyboardWillShow:(NSNotification*)notification;
- (void)keyboardDidHide:(NSNotification*)notification;

/// Scroll scrollView to make view visible.
/// @param view The view to be scrolled to visible.
/// @param topMargin The space to be left above the view.
/// @param bottomMargin The space to be left below the view.
/// @note ActiveView instances can easily use the [ActiveView postScrollToVisibleNotification] to achieve the same result.
- (void)scrollViewToVisible:(UIView *)view
                  topMargin:(CGFloat)topMargin
               bottomMargin:(CGFloat)bottomMargin;

- (NSString*)findAnalyticsTag; // Returns the tag if it exists, otherwise asks the delegate. Nil if all else fails

@end

///// Analytics convenience methods for ActiveView
//@interface ActiveView (Analytics)
//
///// Convenience method to set view's related UIViewController's analyticsTag.
/////
///// The view's viewController should be a ScrollViewController subclass.
///// @param tag Any NSString to be used as tag
///// @see ScrollViewController
///// @see AnalyticsHelper
//- (void)setViewControllerAnalyticsTag:(NSString *)tag;
//
//@end

