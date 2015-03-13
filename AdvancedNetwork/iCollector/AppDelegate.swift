//
//  AppDelegate.swift
//  iCollector
//
//  Created by Nhan Nguyen on 2/12/15.
//  Copyright (c) 2015 Advanced Network. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {
    
    var window: UIWindow?
    
    var locationCollector = LocationCollector()
    
    func application(application: UIApplication, didFinishLaunchingWithOptions launchOptions: [NSObject: AnyObject]?) -> Bool {
        // Override point for customization after application launch.
        var alert = UIAlertView()
        var myapp = UIApplication.sharedApplication()
        if (myapp.backgroundRefreshStatus == UIBackgroundRefreshStatus.Denied) {
            alert.title = "Alert"
            alert.message = "You need to turn Background App Refresh on. To do it, go to Settings > General > Background App Refresh"
            alert.addButtonWithTitle("Gotcha")
            alert.show()
        } else if (myapp.backgroundRefreshStatus == UIBackgroundRefreshStatus.Restricted) {
            
        } else {
            
            locationCollector.startLocationCollecting()
            let time : NSTimeInterval = 600.0
            var locationUpdateTimer = NSTimer.scheduledTimerWithTimeInterval(time, target: self,
                selector: Selector("updateLocation"), userInfo: nil, repeats: true)
            
        }
        
        return true
    }
    
    func updateLocation() {
        println("updating location")
        locationCollector.updateLocationToServer()
    }
    
    
    func applicationWillResignActive(application: UIApplication) {
        // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
        // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
    }
    
    func applicationDidEnterBackground(application: UIApplication) {
        // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
        // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    }
    
    func applicationWillEnterForeground(application: UIApplication) {
        // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    }
    
    func applicationDidBecomeActive(application: UIApplication) {
        // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    }
    
    func applicationWillTerminate(application: UIApplication) {
        // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    }
    
    
}

