google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawChart1);
google.charts.setOnLoadCallback(drawChart2);

function drawChart1() {
	var data = new google.visualization.DataTable();
	data.addColumn('string', 'Topping');
	data.addColumn('number', 'Slices');
	data.addRows([
		['Java', 1388],
		['C', 1337],
		['C++', 1371],
		['Python', 1285],
		['C#', 1347],
		['Visual Basic .NET', 1434],
		['JavaScript', 1220],
		['PHP', 969],
		['Perl', 606],
		['Assembly language', 538],
	]);

	var options = {
		'title':"Amazon's book storing rate of Programing Language",
		'width':400,
		'height':300
	};

	var chart = new google.visualization.PieChart(document.getElementById('chart_div'));
	chart.draw(data, options);
}
function drawChart2() {
	var data = google.visualization.arrayToDataTable([
		["Element", "Star", { role: "style" } ],
		['Java', 3.945172910662811, "#FE642E"],
		['C', 3.9637247569184626, "#FACC2E"],
		['C++', 3.8164113785557894, "#C8FE2E"],
		['Python', 3.9273151750972843, "#64FE2E"],
		['C#', 3.84142538975501, "#2EFE64"],
		['Visual Basic .NET', 4.004114365411432, "#00FF80"],
		['JavaScript', 3.924836065573769, "#2EFEF7"],
		['PHP', 3.9266253869969088, "#2E64FE"],
		['Perl', 3.9353135313531444, "#CC2EFA"],
		['Assembly language', 3.9429368029739917, "#FE2E64"],
	]);

	var view = new google.visualization.DataView(data);
	view.setColumns([0, 1, {
		calc: "stringify",
		sourceColumn: 1,
		type: "string",
		role: "annotation" }, 2
	]);

	var options = {
		title: "Amazon stars statistics for Programming Language books",
		width: 600,
		height: 400,
		bar: { groupWidth: "95%"},
		legend: { position: "none" },
	};
	var chart = new google.visualization.BarChart(document.getElementById("barchart_values"));
	chart.draw(view, options);
}
