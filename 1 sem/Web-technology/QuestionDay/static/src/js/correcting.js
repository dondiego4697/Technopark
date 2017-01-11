

function funcSuccess0(data) {
    console.log(data);
}

function sendAjax0(value, id, csrftoken) {
    $.ajax({
        url: "/correct/",
        type: "POST",
        data: ({csrfmiddlewaretoken: csrftoken, value: value, id: id}),
        success: funcSuccess0
    });
    return false;
}


$('.js-correct').on('click', function () {
    var csrftoken = getCookie('csrftoken');
    var id = $(this).data('id');
    var value = $(this).prop('checked');
    sendAjax0(value, id, csrftoken);
});
