package mathExtra2

// генерирует набор pointsNum равномерно распределенных точек
// в диапазоне [start;end]
func UniformDistributedData(pointsNum int64, start float64, end float64) []float64 {
	step := (end - start) / float64(pointsNum-1)

	x := make([]float64, pointsNum)

	for i := range x {
		if i == 0 {
			x[i] = start
		} else {
			x[i] = x[i-1] + step
		}
	}

	return x
}

// генерирует набор pointsNum равномерно распределенных точек для данной функции
// в диапазоне [start;end]
func UniformDistributedPoints(func1 func(x float64) float64, pointsNum int64, start float64, end float64) ([]float64, []float64) {
	step := (end - start) / float64(pointsNum-1)

	x := make([]float64, pointsNum)
	y := make([]float64, pointsNum)

	for i := range x {
		if i == 0 {
			x[i] = start
		} else {
			x[i] = x[i-1] + step
		}
		y[i] = func1(x[i])
	}

	return x, y
}
