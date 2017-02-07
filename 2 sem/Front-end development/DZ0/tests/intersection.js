'use strict';

QUnit.module('Тестируем функцию intersection', function () {
	const arrayCompare = function (actual, expected, assert) {
		assert.ok(actual.length === expected.length, `Длина полученного массива не совпадает с длиной ожидаемого (${expected.length}!==${actual.length})`);

		for (let pos = 0; pos < actual.length; pos++) {
			const el = actual[pos];

			let count1 = 0;
			let count2 = 0;

			for (let i = 0; i < actual.length; i++) {
				if (Object.is(actual[i], el)) {
					count1++;
				}
				if (Object.is(expected[i], el)) {
					count2++;
				}
			}

			assert.ok(count1 === count2, `Количество элементов '${JSON.stringify(el)}' не совпадает в ожидаемом и полученном массивах (${count2} и ${count1})`);
		}
	};

	QUnit.test('intersection работает с единственным массивом', function (assert) {
		arrayCompare(intersection([]), [], assert);
		arrayCompare(intersection([1]), [1], assert);
		arrayCompare(intersection([42]), [42], assert);
		arrayCompare(intersection(['a']), ['a'], assert);
		arrayCompare(intersection(['a', false]), ['a', false], assert);
		arrayCompare(intersection([NaN, Infinity]), [Infinity, NaN], assert);
		arrayCompare(intersection([NaN, true, 'a', -1]), [NaN, true, 'a', -1], assert);
	});

	QUnit.test('intersection работает с одинаковыми массивами', function (assert) {
		arrayCompare(intersection([], []), [], assert);
		arrayCompare(intersection([1], [1]), [1], assert);
		arrayCompare(intersection([42], [42], [42], [42]), [42], assert);
		arrayCompare(intersection(['a'], ['a']), ['a'], assert);
		arrayCompare(intersection(['a', false], [false, 'a']), ['a', false], assert);
		arrayCompare(intersection([NaN, Infinity, NaN], [NaN, Infinity, NaN]), [Infinity, NaN, NaN], assert);
	});

	QUnit.test('intersection работает с разными массивами', function (assert) {
		arrayCompare(intersection([], [1], [2]), [], assert);
		arrayCompare(intersection([1], [1], [1, 2]), [1], assert);
		arrayCompare(intersection([1, 2, 3], [1, 2, 4], [1, 2, 5]), [2, 1], assert);
		arrayCompare(intersection([1, 2, 2], [2, 2, 3], [1, 2, 2, 5], [null, 2, '2', 2, false]), [2, 2], assert);
		arrayCompare(intersection(['1', '2', '3', true], [1, 2, 3, true], [1, 2, '3', false]), [], assert);
        arrayCompare(intersection(['ads', Infinity, NaN, -1, false, true], [true, 'ads', 'sads', NaN]), ['ads', NaN, true], assert);
    });
});
