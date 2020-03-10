package pltExtra

import (
	"image/color"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
)

// создает график и возвращает указатель на него
func CreatePlot(name string, xName string, yName string) *plot.Plot {
	p, err := plot.New()
	if err != nil {
		panic(err)
	}

	p.Title.Text = name
	p.X.Label.Text = xName
	p.Y.Label.Text = yName

	p.Add(plotter.NewGrid())

	return p
}

// строит на данном p Plot график данной функции по 100 сгенерированным равномерным точкам
func AddLineByFunc(p *plot.Plot, name string, func1 func(x float64) float64, typeOfLine string, col color.Color) {
	data := Points(func1, 100)
	graph, err := plotter.NewLine(data)
	if err != nil {
		panic(err)
	}
	graph.LineStyle.Width = vg.Points(1)
	graph.LineStyle.Color = col

	switch typeOfLine {
	case "long dash":
		graph.LineStyle.Dashes = []vg.Length{vg.Points(15), vg.Points(15)}
	case "small dash":
		graph.LineStyle.Dashes = []vg.Length{vg.Points(5), vg.Points(5)}
	case "line":
	}

	p.Add(graph)
	p.Legend.Add(name, graph)
}

// Добавляет на график функцию построенную линейной аппроксимацией по точкам xNodes, yNodes
func AddLineByPoints(p *plot.Plot, name string, xNodes []float64, yNodes []float64, typeOfLine string, col color.Color) {
	data := PointsByMy(xNodes, yNodes)

	graph, err := plotter.NewLine(data)
	if err != nil {
		panic(err)
	}

	graph.LineStyle.Width = vg.Points(1)
	graph.LineStyle.Color = col

	switch typeOfLine {
	case "long dash":
		graph.LineStyle.Dashes = []vg.Length{vg.Points(10), vg.Points(10)}
	case "small dash":
		graph.LineStyle.Dashes = []vg.Length{vg.Points(4), vg.Points(4)}
	default:
	}

	p.Add(graph)
	p.Legend.Add(name, graph)

}

// Добавляет на график точки xNodes, yNodes
func AddScatterByPoints(p *plot.Plot, name string, xNodes []float64, yNodes []float64, col color.Color) {
	data := PointsByMy(xNodes, yNodes)

	scatter, err := plotter.NewScatter(data)
	if err != nil {
		panic(err)
	}
	scatter.GlyphStyle.Color = col

	p.Add(scatter)
	p.Legend.Add(name, scatter)
}

// сохраняет график с заданным именем в папке ./plots/ в формате png и svg(в большем размере)
func SavePlotToFiles(p *plot.Plot, name string) {
	if err := p.Save(12*vg.Centimeter, 12*vg.Centimeter, "./plots/"+name+".png"); err != nil {
		panic(err)
	}
	if err := p.Save(20*vg.Centimeter, 20*vg.Centimeter, "./plots/"+name+".svg"); err != nil {
		panic(err)
	}
}

// Вычисляет значения переданной функции в pointsNum равномерно распределенных узлах для построения графика
func Points(func1 func(x float64) float64, pointsNum int64) plotter.XYs {
	var start float64 = -1.0
	var end float64 = 1.0

	step := (end - start) / float64(pointsNum-1)

	pts := make(plotter.XYs, pointsNum)
	for i := range pts {
		if i == 0 {
			pts[i].X = start
		} else {
			pts[i].X = pts[i-1].X + step
		}
		pts[i].Y = func1(pts[i].X)
	}

	return pts
}

// Преобразует значения переданных точек в формат для рисования на графике
func PointsByMy(xNodes []float64, yNodes []float64) plotter.XYs {
	pointsNum := int64(len(xNodes))

	pts := make(plotter.XYs, pointsNum)

	for i := range pts {
		pts[i].X = xNodes[i]
		pts[i].Y = yNodes[i]
	}

	return pts
}
