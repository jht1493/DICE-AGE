
	report = { };
	report.win = w;
	resetReport(report);

function resetReport(report) 
{
	report.frameCount = 0;
	if (report == undefined) return;