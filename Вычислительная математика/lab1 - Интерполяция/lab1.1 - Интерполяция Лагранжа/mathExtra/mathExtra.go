package mathExtra

import (
	"math"
	"math/rand"
	"time"
)

// инициализирует псевдослучайную последовательность
func RandInit() {
	rand.Seed(time.Now().UnixNano())
}

// возвращает случайное целое число в диапазоне [from;to]
func RandInt64(from int64, to int64) int64 {
	return int64(rand.Intn(int(to-from+1))) + from
}

// возвращает случайное вещественное число в диапазоне [from;to]
func RandFloat64(from int64, to int64) float64 {
	return float64(RandInt64(from*10000, to*10000)) / 10000
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

// генерирует набор pointsNum чебышёвских узлов для данной функции
// в диапазоне [-1;1]
func ChebDistributedPoints(func1 func(x float64) float64, pointsNum int64) ([]float64, []float64) {
	x := make([]float64, pointsNum)
	y := make([]float64, pointsNum)

	for i := 1; i <= int(pointsNum); i++ {
		x[i-1] = math.Cos((2.0*float64(i) - 1) / (2.0 * float64(pointsNum)) * math.Pi)
		y[i-1] = func1(x[i-1])
	}

	return x, y
}
