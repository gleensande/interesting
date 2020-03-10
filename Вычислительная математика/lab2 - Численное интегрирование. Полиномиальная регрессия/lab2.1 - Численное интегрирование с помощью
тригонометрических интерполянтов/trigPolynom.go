package main

import (
	"fmt"
	"math"
	"math/cmplx"

	myMath2 "./mathExtra2"
)

const I complex128 = complex(0, 1)
const PI = math.Pi
const PI2 = PI * PI
const E = math.E

func main() {
	// анализ интегралов
	integrals_analysis()
}

// определяет все коэффициенты тригонометрического ряда
// для заданных узлов y_nodes (2)
func fft_coeff(y_nodes []complex128) []complex128 {
	m := int64(len(y_nodes))
	coeffs := make([]complex128, m)

	var k int64
	for k = 0; k < m; k++ {
		coeffs[k] = cmplx.Pow((-1), complex(float64(k), 0))
		coeffs[k] /= complex(float64(m), 0)

		coeffs[k] *= Cooley_Tukey(y_nodes, k)
	}
	return coeffs
}

// алгоритм Кули-Тьюки для нахождения одной суммы Ak (2)
func Cooley_Tukey(y_nodes []complex128, k int64) complex128 {
	m := int64(len(y_nodes) / 2)

	if m == 1 {
		return (y_nodes[0] + y_nodes[1]*complex(math.Pow(-1, float64(k)), 0))
	}

	Ek := make([]complex128, m)
	Ok := make([]complex128, m)

	var i int64
	var j int64 = 0
	for i = 0; i < m; i++ {
		Ek[i] = y_nodes[j]
		Ok[i] = y_nodes[j+1]
		j += 2
	}

	return (Cooley_Tukey(Ek, k) + cmplx.Pow(E, -I*complex(float64(k)*PI/float64(m), 0))*Cooley_Tukey(Ok, k))
}

// находит значение интеграла для функции f,
// интерполируемой в N точках
func spectral_integral(f func(x float64) float64, N int64) float64 {
	// задание точек
	_, y_nodes0 := myMath2.UniformDistributedPoints(f, N, -PI, PI)
	y_nodes := make([]complex128, N)
	var i int64
	for i = 0; i < N; i++ {
		y_nodes[i] = complex(y_nodes0[i], 0)
	}

	// получение комплексных коэффициентов
	coeffs := fft_coeff(y_nodes)
	// подсчет интеграла по выведенной формуле
	var I float64 = 0
	I += 5 * PI / 4 * real(coeffs[0])
	var k float64
	for k = 1; k < float64(N); k += 1 {
		I += 2 * real(coeffs[int64(k)]) / k * math.Sin(PI*k/4)
		I += 2 * imag(coeffs[int64(k)]) / k * (math.Cos(k*PI) - math.Cos(k*PI/4))
	}

	return I
}

// выполняет анализ интегралов (4)
func integrals_analysis() {
	// точные значения интегралов
	const numOfFuncs = 2
	realResults := make([]float64, numOfFuncs)
	realResults[0] = 17 * (math.Pow(PI, 2)) / 32
	realResults[1] = 0.5*(math.Sin(PI2)-math.Sin(PI2/16)) +
		math.Sin(math.Pow(E, PI)) - math.Sin(math.Pow(E, -PI/4))

	/*fmt.Println("Real results")
	for i := 0; i < numOfFuncs; i++ {
		fmt.Println("I[", i, "]=", realResults[i])
	}*/

	// массив исходных функций
	funcs := make([](func(x float64) float64), numOfFuncs)

	funcs[0] = (func(x float64) float64 {
		return math.Abs(x)
	})

	funcs[1] = (func(x float64) float64 {
		return x*math.Cos(x*x) + math.Pow(E, x)*math.Cos(math.Pow(E, x))
	})

	// нахождение приближенных значений интегралов
	// i - номер функции, j - порядок приближения
	fmt.Println("Approximation results")
	approxResults := make([][10]float64, numOfFuncs)
	for i := 0; i < numOfFuncs; i++ {
		for j := 1; j < 9; j++ {
			N := int64(math.Pow(2, float64(j)))
			approxResults[i][j] = spectral_integral(funcs[i], N)
			// fmt.Println("I[", i, "][", j, "] =", approxResults[i][j])
			fmt.Println(N, " ", math.Abs((approxResults[i][j]-realResults[i])/realResults[i]))
		}
	}

}
