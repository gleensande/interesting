package main

import (
	"image/color"
	"math"
	"strconv"

	myMath "./mathExtra"
	myPlot "./plotExtra"
)

func main() {
	// создание исходных функций аппроксимации Паде
	const numOfFuncs int64 = 8
	numOfPoints := numOfFuncs

	funcsPade := generateSomePadeApproxFuncs(numOfFuncs)

	// графики исходных функций + полиномов Лагранжа
	createSomePadeLagrPlots(funcsPade, numOfPoints)

	// графики норм в зависимости от числа узлов
	createSomeNormPlots(funcsPade)

}

// (1) рассчитывает i-тый базисный полином Лагранжа
func l_i(i int64, x float64, x_nodes []float64) float64 {
	result := 1.0

	var j int64
	for j = 0; j < int64(len(x_nodes)); j++ {
		if j == i {
			continue
		}

		result *= (x - x_nodes[j]) / (x_nodes[i] - x_nodes[j])
	}

	return result
}

// (2) возвращает функцию для полинома Лагранжа, построенного по данным точкам
func L(x_nodes []float64, y_nodes []float64) func(x float64) float64 {
	return (func(x float64) float64 {
		result := 0.0

		if len(x_nodes) != len(y_nodes) {
			panic("lens don't match")
		}

		var i int64
		for i = 0; i < int64(len(x_nodes)); i++ {
			result += y_nodes[i] * l_i(i, x, x_nodes)
		}

		return result
	})
}

// генерирует случайные константы для одной функции
func generateFuncConsts() (int64, int64, []float64, []float64) {
	n := myMath.RandInt64(7, 15)
	m := myMath.RandInt64(7, 15)

	a := make([]float64, m)
	b := make([]float64, n)

	var i int64
	for i = 0; i < m; i++ {
		a[i] = myMath.RandFloat64(0, 1)
	}

	for i = 0; i < n; i++ {
		b[i] = myMath.RandFloat64(0, 1)
	}

	return n, m, a, b
}

// (3) генерирует функцию аппроксимации Паде
func generatePadeApproxFunc() func(x float64) float64 {
	n, m, a, b := generateFuncConsts()

	return (func(x float64) float64 {
		if x > 1 || x < -1 {
			//panic("x out of range")
		}
		var numerator float64
		var denominator float64

		numerator = 0
		denominator = 1

		var j int64
		for j = 0; j < m; j++ {
			numerator += a[j] * math.Pow(x, float64(j))
		}

		var k int64
		for k = 1; k < n; k++ {
			denominator += b[k] * math.Pow(x, float64(k))
		}

		return (numerator / denominator)
	})
}

// генерирует numOfFuncs функций аппроксимации Паде
func generateSomePadeApproxFuncs(numOfFuncs int64) [](func(x float64) float64) {
	myMath.RandInit()

	funcsPade := make([](func(x float64) float64), numOfFuncs)

	var i int64
	for i = 0; i < numOfFuncs; i++ {
		funcsPade[i] = generatePadeApproxFunc()
	}

	return funcsPade
}

// генерирует графики Паде, Лагранжа по numOfPoints равномерным и чебышёвским узлам
// для одной переданной функции аппроксимации Паде
// сохраняет в файл под номером numOfFile
func createPadeLagrPlot(funcPade func(x float64) float64, numOfPoints int64, numOfFile int64) {
	// генерация распределенных узлов и функции Лагранжа по ним
	x_nodes_uni, y_nodes_uni := myMath.UniformDistributedPoints(funcPade, numOfPoints, -1, 1)
	funcLagrUni := L(x_nodes_uni, y_nodes_uni)

	// генерация чебышёвских узлов и функции Лагранжа по ним
	x_nodes_cheb, y_nodes_cheb := myMath.ChebDistributedPoints(funcPade, numOfPoints)
	funcLagrCheb := L(x_nodes_cheb, y_nodes_cheb)

	colors := [4]color.Color{
		color.RGBA{R: 142, G: 166, B: 4, A: 255},
		color.RGBA{R: 236, G: 159, B: 5, A: 255},
		color.RGBA{R: 191, G: 49, B: 0, A: 255},
		color.Black,
	}

	// создание различных линий на графике и сохранение в 2 файла: .png и .svg
	graph := myPlot.CreatePlot("Аппроксимация Паде и полиномы Лагранжа", "x", "f(x)")
	myPlot.AddLineByFunc(graph, "Аппроксимация Паде", funcPade, "line", colors[0])
	myPlot.AddLineByFunc(graph, "Полином Лагранжа по равномерным узлам", funcLagrUni, "long dash", colors[1])
	myPlot.AddLineByFunc(graph, "Полином по узлам Чебышёва", funcLagrCheb, "small dash", colors[2])
	myPlot.AddScatterByPoints(graph, "Узлы Чебышёва", x_nodes_cheb, y_nodes_cheb, colors[3])
	myPlot.SavePlotToFiles(graph, "Функции/Аппроксимация Паде и полиномы Лагранжа ("+strconv.Itoa(int(numOfFile))+")")

}

// (4) генерирует графики Паде, Лагранжа по равномерным и чебышёвским узлам
// по переданным функциям аппроксимации Паде
func createSomePadeLagrPlots(funcsPade [](func(x float64) float64), numOfPoints int64) {
	var numOfFuncs int64 = int64(len(funcsPade))

	var i int64
	for i = 0; i < numOfFuncs; i++ {
		createPadeLagrPlot(funcsPade[i], numOfPoints, i)
	}
}

// рассчитывает полиномы Лагранжа при разном числе узлов
// от start до end для одной функции Паде
func createLagrFuncs(funcPade func(x float64) float64, start int64, end int64) ([](func(x float64) float64), [](func(x float64) float64)) {
	var numOfPoints int64
	numOfPoints = end - start + 1

	funcsLagrUni := make([](func(x float64) float64), numOfPoints)
	funcsLagrCheb := make([](func(x float64) float64), numOfPoints)

	var n int64
	var i int64 = 0
	for n = start; n <= end; n++ {
		x_nodes_uni, y_nodes_uni := myMath.UniformDistributedPoints(funcPade, n, -1, 1)
		funcsLagrUni[i] = L(x_nodes_uni, y_nodes_uni)
		x_nodes_cheb, y_nodes_cheb := myMath.ChebDistributedPoints(funcPade, n)
		funcsLagrCheb[i] = L(x_nodes_cheb, y_nodes_cheb)
		i++
	}

	return funcsLagrUni, funcsLagrCheb
}

// рассчитывает равномерную норму для двух функций по 100 равномерно расположенным точкам
func computeNorm(funcPade func(x float64) float64, funcLagr func(x float64) float64) float64 {
	_, pointsYPade := myMath.UniformDistributedPoints(funcPade, 100, -1, 1)
	_, pointsYLagr := myMath.UniformDistributedPoints(funcLagr, 100, -1, 1)

	var max float64 = 0
	for i := range pointsYPade {
		max = math.Max(max, math.Abs(pointsYPade[i]-pointsYLagr[i]))
	}

	return max
}

// рассчитывает полиномы Лагранжа, нормы по отношению к Паде и строит их графики
// для одной функции Паде
func createNormPlot(funcPade func(x float64) float64, num int) {
	var start int64 = 1
	var end int64 = 30

	// расчет функций Лагранжа при различном количестве узлов
	funcsLagrUni, funcsLagrCheb := createLagrFuncs(funcPade, start, end)

	numOfFuncs := start + end - 1

	normsUni := make([]float64, numOfFuncs)
	normsCheb := make([]float64, numOfFuncs)

	var i int64
	for i = 0; i < numOfFuncs; i++ {
		// расчет норм по отношению к функции Паде для каждой из функций Лагранжа
		normsUni[i] = computeNorm(funcPade, funcsLagrUni[i])
		normsCheb[i] = computeNorm(funcPade, funcsLagrCheb[i])
	}

	colors := [3]color.Color{
		color.RGBA{R: 142, G: 166, B: 4, A: 255},
		color.RGBA{R: 236, G: 159, B: 5, A: 255},
	}

	xNodes := make([]float64, start+end-1)
	for i = 0; i < start+end-1; i++ {
		xNodes[i] = float64(i + 1)
	}

	// построение графиков на одной координатной оси для двух зависимостей норм
	plotNorms := myPlot.CreatePlot("Зависимость нормы от числа узлов интерполяции", "Число узлов", "Значение нормы")
	myPlot.AddLineByPoints(plotNorms, "равномерно распределенные узлы", xNodes, normsUni, "small dash", colors[0])
	myPlot.AddLineByPoints(plotNorms, "узлы Чебышёва", xNodes, normsCheb, "line", colors[1])
	myPlot.SavePlotToFiles(plotNorms, "Нормы/Зависимость нормы от числа узлов интерполяции для функции ("+strconv.Itoa(num)+")")
}

// (5) используя функцию построения графиков норм, строит нормы для каждой из функций Паде
func createSomeNormPlots(funcsPade []func(x float64) float64) {
	for i := range funcsPade {
		createNormPlot(funcsPade[i], i)
	}
}
