//For getting CSRF token
function getCookie(name) {
    var cookieValue = null;
    if (document.cookie && document.cookie != '') {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
            var cookie = jQuery.trim(cookies[i]);
            // Does this cookie string begin with the name we want?
            if (cookie.substring(0, name.length + 1) == (name + '=')) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}

function funcSuccess(data) {
    console.log(data);
    json = data;
    if (json.result == 'Success') {
        if (json.type == 'a') {
            var ratingA = document.getElementById("a" + json.id);
            ratingA.innerHTML = json.rating;
        } else if (json.type == 'q') {
            var ratingQ = document.getElementById("q" + json.id);
            ratingQ.innerHTML = json.rating;
        }
    } else if (json.result == 'Error') {

    } else if (json.result == 'Already') {
        alert('Already voted!');
    }
}

function funcError() {
    console.log('ERROR!');
}

function sendAjax(vote, type, id, csrftoken) {
    $.ajax({
        url: "/vote/",
        type: "POST",
        data: ({csrfmiddlewaretoken: csrftoken, vote: vote, type: type, id: id}),
        success: funcSuccess,
        error: funcError
    });
    return false;
}

$('.js-like').on('click', function () {
    var csrftoken = getCookie('csrftoken');
    var id = $(this).data('id');
    var type = $(this).data('type');
    var vote = $(this).data('vote');
    sendAjax(vote, type, id, csrftoken);
    return false;
});




