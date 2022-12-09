window.addEventListener("load", getBois());

setInterval(function getTemperatureSensor(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function(){
        if(this.status == 200){
            document.getElementById("temperature").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lireTemperature", true);
    xhttp.send();
}, 3000);

function getBois(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function(){
        if(this.status == 200) {
            if(this.responseText.length > 0) {     
                var arrayBois = this.responseText.split("&");
                for(var i = 0; i < arrayBois.length - 1; i = i+2) {
                    var idBois = arrayBois[i];
                    var nomBois = arrayBois[i+1];
                    
                    document.getElementById("typeBois").innerHTML += "<option value='" + idBois + "'>" + nomBois + "</option>";
                }
                document.getElementById("typeBois").addEventListener("change", getCaracteristiqueBois());
                window.addEventListener("load", getCaracteristiqueBois());
            }
        }
    };
xhttp.open("GET", "getBois", true);
xhttp.send();
}

function getCaracteristiqueBois(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function(){
        if(this.status == 200) {
            alert(this.responseText.length);
            if(this.responseText.length > 0) { 
                var arrayBois = this.responseText.split("&");
                for(var i = 0; i < arrayBois.length - 1; i = i+5) {
                    var nomDuBois = arrayBois[i];
                    var typeDuBois = arrayBois[i+1];
                    var origineDuBois = arrayBois[i+2];
                    var tempsDuBois = arrayBois[i+3];
                    var temperatureDuBois = arrayBois[i+4];
                    alert(nomDuBois);
                    alert(typeDuBois);
                    alert(origineDuBois);
                    alert(tempsDuBois);
                    alert(temperatureDuBois);
                    document.getElementById("nomDuBois").innerHTML += nomDuBois;
                    document.getElementById("typeDuBois").innerHTML += typeDuBois;
                    document.getElementById("origineDuBois").innerHTML += origineDuBois;
                    document.getElementById("tempsDuBois").innerHTML += tempsDuBois;
                    document.getElementById("temperatureDuBois").innerHTML += temperatureDuBois;
                }
                // window.addEventListener("load", getCaracteristiqueBois());
            }
        }
    };
xhttp.open("GET", "getCaracteristiqueBois", true);
xhttp.send();
}

// var boisSelectionne;
// var temperature;
// function four(){
//     var i = 0;
//     var temp = parseInt(temperature);
//     if( temp >= boisSelectionne.tempMin){
//         var timer = setInterval(function(){
//             i++
//             document.getElementById("timer").innerHTML = i;
//             console.log(i);
//             if(i == boisSelectionne.drying){
//                 clearInterval(timer);
//             }
            
//         }, 1000);
//     } else {
//         console.log('non');
//     }
// };