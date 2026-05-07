package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// Метод Борда
func bordaMethod(candidates []string, rankings [][]int, votesCount []int) {
	n := len(candidates)
	scores := make(map[string]int)

	for _, cand := range candidates {
		scores[cand] = 0
	}

	// Подсчёт очков
	for place := 0; place < len(rankings); place++ {
		for col := 0; col < len(rankings[place]); col++ {
			candidateIndex := rankings[place][col]
			points := n - 1 - place
			scores[candidates[candidateIndex]] += points * votesCount[col]
		}
	}

	// Поиск победителя
	winner := ""
	maxScore := -1
	for cand, score := range scores {
		if score > maxScore {
			maxScore = score
			winner = cand
		}
	}

	fmt.Printf("По Борду: %s (%d очков)\n", winner, maxScore)
}

// Метод Кондорсе
func condorcetMethod(candidates []string, rankings [][]int, votesCount []int) {
	n := len(candidates)
	m := len(votesCount)
	wins := make([][]int, n)
	for i := range wins {
		wins[i] = make([]int, n)
	}

	// Подсчёт парных сравнений
	for group := 0; group < m; group++ {
		for i := 0; i < n; i++ {
			for j := i + 1; j < n; j++ {
				candidateHigher := rankings[i][group]
				candidateLower := rankings[j][group]
				wins[candidateHigher][candidateLower] += votesCount[group]
			}
		}
	}

	// Поиск победителя Кондорсе
	winner := ""
	found := false

	for i := 0; i < n; i++ {
		beatsAll := true
		for j := 0; j < n; j++ {
			if i != j {
				if wins[i][j] <= wins[j][i] {
					beatsAll = false
					break
				}
			}
		}
		if beatsAll {
			winner = candidates[i]
			found = true
			break
		}
	}

	if found {
		fmt.Printf("По Кондорсе: %s\n", winner)
	} else {
		fmt.Println("По Кондорсе: не определён")
	}
}

// Вывод таблицы
func printTable(candidates []string, votesCount []int, rankings [][]int) {
	n := len(candidates)
	m := len(votesCount)

	// Фиксированная ширина колонок
	firstColWidth := 25
	dataColWidth := 10

	// Функция для отрисовки горизонтальной линии
	drawLine := func() {
		fmt.Print("+-" + strings.Repeat("-", firstColWidth) + "-+")
		for i := 0; i < m; i++ {
			fmt.Print(strings.Repeat("-", dataColWidth+1) + "+")
		}
		fmt.Println()
	}

	fmt.Println("\nРезультаты ранжирования:")

	drawLine()

	// Шапка таблицы
	fmt.Printf("| %-*s |", firstColWidth, "Количество избирателей")
	for _, count := range votesCount {
		fmt.Printf(" %*d  |", dataColWidth-1, count)
	}
	fmt.Println()

	drawLine()

	// Строки таблицы
	for place := 0; place < n; place++ {
		points := n - 1 - place
		placeText := fmt.Sprintf("%d-е место (%d)", place+1, points)

		fmt.Printf("| %-*s |", firstColWidth, placeText)
		for col := 0; col < m; col++ {
			candName := candidates[rankings[place][col]]
			fmt.Printf(" %-*s  |", dataColWidth-1, candName)
		}
		fmt.Println()
	}

	drawLine()
}

// Ввод данных
func inputData() ([]string, []int, [][]int) {
	reader := bufio.NewReader(os.Stdin)

	fmt.Println("Теория голосований")

	// Ввод количества кандидатов
	fmt.Print("\nВведите количество кандидатов: ")
	nStr, _ := reader.ReadString('\n')
	n, _ := strconv.Atoi(strings.TrimSpace(nStr))

	candidates := make([]string, n)
	fmt.Println("\nВведите имена кандидатов:")
	for i := 0; i < n; i++ {
		fmt.Printf("  Кандидат %d: ", i+1)
		name, _ := reader.ReadString('\n')
		candidates[i] = strings.TrimSpace(name)
	}

	// Ввод количества групп избирателей
	fmt.Print("\nВведите количество групп избирателей: ")
	mStr, _ := reader.ReadString('\n')
	m, _ := strconv.Atoi(strings.TrimSpace(mStr))

	votesCount := make([]int, m)
	fmt.Println("\nВведите количество избирателей в каждой группе:")
	for i := 0; i < m; i++ {
		fmt.Printf("  Группа %d: ", i+1)
		countStr, _ := reader.ReadString('\n')
		votesCount[i], _ = strconv.Atoi(strings.TrimSpace(countStr))
	}

	rankings := make([][]int, n)
	for i := range rankings {
		rankings[i] = make([]int, m)
	}

	fmt.Println("\nВвод предпочтений избирателей")
	fmt.Print("Кандидаты: ")
	for _, cand := range candidates {
		fmt.Print(cand, " ")
	}
	fmt.Println("\n")

	for place := 0; place < n; place++ {
		fmt.Printf("%d-е место:\n", place+1)
		for group := 0; group < m; group++ {
			fmt.Printf("  Группа %d (%d изб.): ", group+1, votesCount[group])
			candidateName, _ := reader.ReadString('\n')
			candidateName = strings.TrimSpace(candidateName)

			// Поиск индекса кандидата
			idx := -1
			for i, cand := range candidates {
				if cand == candidateName {
					idx = i
					break
				}
			}

			if idx != -1 {
				rankings[place][group] = idx
			} else {
				fmt.Println("    Ошибка! Кандидат не найден. Повторите ввод.")
				group--
			}
		}
		fmt.Println()
	}

	return candidates, votesCount, rankings
}

func main() {
	candidates, votesCount, rankings := inputData()
	printTable(candidates, votesCount, rankings)

	fmt.Println()
	bordaMethod(candidates, rankings, votesCount)
	condorcetMethod(candidates, rankings, votesCount)
}
