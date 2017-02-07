'use strict';

QUnit.module('Тестируем функцию max', function () {
	QUnit.test('Возвращает максимальное из трёх положительных чисел', function (assert) {
		assert.strictEqual(max([1, 2, 3]), 3, 'max([1, 2, 3]) === 3');
		assert.strictEqual(max([3, 2, 1]), 3, 'max([3, 2, 1]) === 3');
	});

	QUnit.test('Возвращает максимальное из трёх отрицательных чисел', function (assert) {
		assert.strictEqual(max([-1, -2, -3]), -1, 'max([-1, -2, -3]) === -1');
		assert.strictEqual(max([-3, -2, -1]), -1, 'max([-3, -2, -1]) === -1');
	});

	QUnit.test('Возвращает максимальное из трёх чисел разных знаков', function (assert) {
		assert.strictEqual(max([-1, 0, 1]), 1, 'max([-1, 0, 1]) === 1');
		assert.strictEqual(max([1, 0, -1]), 1, 'max([1, 0, -1]) === 1');
	});
});
