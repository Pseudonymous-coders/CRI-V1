var nameProgs = [];
var progs = "";

$('#siteNav').affix({
    offset: {
        top: 100
    }
})

function progClicked(id) {
    document.querySelector('.notify').innerHTML = "Removing "+id;
    setTimeout(function() {
        document.querySelector('.notify').innerHTML = "<a href='index.html'>CRI</a>";
    }, 3000);
}

//document.getElementById('items').innerHTML += "<button onclick='progClicked(this.id)' class='item' id='"+progName+"'><img height='50' width='50' src='images/icons/"+iconName+".png'><br>"+progName+"</button>";
