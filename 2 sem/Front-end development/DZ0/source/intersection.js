'use strict';

/*Метод заключается в том, что берется первый входящий массив(resultArr) и все последующие сравниваются с ним.
* По мере сравнения из массива resultArr удаляются непересекающиеся элементы.*/

const intersection = function () {
    const arrays = Array.from(arguments);
    const resultArr = arrays[0];
    if (resultArr.length <= 1) {
        return resultArr;
    } else {
        for (let i = 1; i < arrays.length; i++) {
            const currArr = arrays[i];
            let j = 0;
            while (j < resultArr.length) {
                const resultItem = resultArr[j];
                let findCheck = false;
                for (let k = 0; k < currArr.length; k++) {
                    if (resultItem === currArr[k] || (isNaN(resultItem) && isNaN(currArr[k]))) {
                        findCheck = true;
                        currArr.splice(k, 1);
                        break;
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
