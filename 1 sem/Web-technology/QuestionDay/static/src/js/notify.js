function done(data) {
	var json = data;
    var div = document.getElementById("new-answers");
    var block;
    block = '<div class="row card" style="background-color: #b1d4ff"><div class="col-sm-3">' +
        '<div class="form-group margin-bottom"><div class="div-inline-block">' +
        '</div></div></div><div class="col-sm-9 text-left"><h4 class="custom-text-left">' + json.text + '</h4></div>';

    $(div).prepend(block); 
    handle_comet();
}

function error(data) {
    //alert('ERROR');
    handle_comet();
}

function handle_comet() {
    $.ajax({
        url: "/sub/1",
        type: "GET",
        dataType: 'json',
        success: done,
        error: error
    });
}

handle_comet();
