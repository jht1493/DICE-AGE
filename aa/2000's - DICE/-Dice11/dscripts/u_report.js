function setupReportWindow(){	var w = new Window( 640+80 /*-left-*/, 480 + 80 /*-top-*/, 400 /*-width-*/, 100 /*-height-*/ )	w.ctl1 = new TextField( w, "This is a test ", 0,  0, 380, 22 );	w.ctl1.visible = 1	w.show();
	report = { };
	report.win = w;
	resetReport(report);	return report;}

function resetReport(report) 
{
	report.frameCount = 0;	report.startTime = GetCurrentEventTime();	report.lastNow = report.startTime;}function issueReport(report){
	if (report == undefined) return;	report.frameCount++;	var now2 = GetCurrentEventTime();	var lapse = now2 - report.startTime;	var fps = report. frameCount / lapse ;			if (report.win != undefined && (now2 - report.lastNow) > 1.0 )	{		lapse = Math.round( lapse * 100 ) / 100;		fps = Math.round( fps * 100 ) / 100;		//report_win.ctl1.title = "Lapse "+lapse+" Frame "+frameCount+" Fps "+fps;		report.win.ctl1.title = " Fps "+fps;		report.lastNow = now2;	}}