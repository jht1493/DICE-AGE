// Simple test of displaying Digitizing Cameraimg = new Image();cam = new Camera();cam.output = img;win = new Window(5, 50, 640, 480);win.show();proj = new Projector();proj.input = img;proj.output = win;function setupReport(){	var w = new Window( 640+80 /*-left-*/, 480 + 80 /*-top-*/, 400 /*-width-*/, 100 /*-height-*/ )	w.ctl1 = new TextField( w, "This is a test ", 5, 10, 380, 22 );	w.ctl1.visible = 1	w.show();	return w;}var win2;win2 = setupReport();frameCount = 0;startTime = GetCurrentEventTime();lastNow = startTime;processThem();function processThem(){	cam.process();	proj.process();	frameCount++;	now2 = GetCurrentEventTime();	lapse = now2 - startTime;	fps = frameCount / lapse ;			if (win2 != undefined && (now2 - lastNow) > 1.0 )	{		//win2.ctl1.title = "Lapse "+lapse+" Frame "+frameCount+" Fps "+fps;		win2.ctl1.title = " Fps "+fps.toFixed(2);		lastNow = now2;	}}function spendTime(){	processThem();}