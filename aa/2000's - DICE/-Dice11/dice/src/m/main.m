//
//  main.m
//  doc
//
//  Created by jht1 on 8/23/06.
//  Copyright __MyCompanyName__ 2006 . All rights reserved.
//

#import <Cocoa/Cocoa.h>

extern int
js_main(int argc, char **argv, char **envp);

int main(int argc, char *argv[])
{
    return NSApplicationMain(argc, (const char **) argv);
	//return js_main(argc, argv, 0);
}
