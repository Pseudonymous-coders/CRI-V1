items = new Array(); 
var name = "";
var desc = "";
var sntnc = "";
$('#siteNav').affix({
    offset: {
        top: 100
    }
})

document.getElementById('items').style.visibility = "hidden";

function search() {
    document.getElementById('items').innerHTML = "";
    toSearch = document.getElementById('text-search').value;
    if (toSearch != "") {
        document.getElementById('items').innerHTML = "<p class='item'>Would search "+toSearch+" but this is simply your demo</p>";
    }
    document.getElementById('items').style.visibility = "visible";
}

$('#text-search').keypress(function (e) {
    var code = e.keyCode || e.which;
    if (code === 13) {
        search();
    };
});
