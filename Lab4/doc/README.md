Приложение разработано под Windows.

Приложение реализует следующие функции:

1. Выбор параметров отрезка и окружности для растеризации (цвет, координаты точек и радиус)
2. Изменение масштаба
3. Возможность использования задежки при рисовании для иллюстрации работы алгоритмов
4. Растеризация отрезка пошаговым алгоритмом, алгоритмом ЦДА и алгоритмом Брезенхема
5. Растеризация окружности алгоритмом Брезенхема

Способы реализации представленных функций

Задание параметров для растеризации и масштабирования происходит в соответствующих полях.

Задержка, которая задается неотрицательным числовым значением (единицы измерения - мс), реализована с помощью функции msleep(). Она позволяет отследить каждый шаг работы алгоритма за счет отображения промежуточных данных. 

Работа с графиками для отображения работы алгоритмов реализована с помощью библиотеки qcustomplot. Реализация предполагает, что при запуске какого-либо алгоритма координатная сетка не содержит нарисованных ранее графиков.

Примечания:
1. Оценка времени работы непосредственно алгоритмов растеризации имеет асимптотику O(x2-x1).
2. Время, затраченное на работу алгоритмов, выводится с помощью qDebug().
