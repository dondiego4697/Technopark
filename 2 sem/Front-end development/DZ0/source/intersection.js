'use strict';

/*Метод заключается в том, что берется первый входящий массив(resultArr) и все последующие сравниваются с ним.
* По мере сравнения из массива resultArr удаляются непересекающиеся элементы.*/

const intersection = function () {
    var arrays = Array.from(arguments);
    var resultArr = arrays[0];
    if (resultArr.length <= 1) {
        return resultArr;
    } else {
        for (var i = 1; i < arrays.length; i++) {
            let currArr = arrays[i];
            var j = 0;
            while (j < resultArr.length) {
                var resultItem = resultArr[j];
                var findCheck = false;
                for (var k = 0; k < currArr.length; k++) {
                    if (resultItem === currArr[k] || (isNaN(resultItem) && isNaN(currArr[k]))) {
                        findCheck = true;
                        currArr.splice(k, 1);
                        break;
                    } else {

                    }
                }
                if (!findCheck) {
                    resultArr.splice(j, 1);
                } else {
                    j++;
                }
            }
        }
    }
    return resultArr;
};
