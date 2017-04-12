
fs = require('fs')

var express = require('express');
var app = express();



app.get('/', function (req, res) {

var file='';
if(req.query.q)
	file=req.query.q
else
	file='01042017'

console.log('./data/'+file+'.txt');
fs.readFile('./data/'+file+'.txt', 'utf8', function (err,data) {
  if (err)  return console.log(err);
  
  var labels =[];
  var temperature1 =[];
  var light=[];
  var tempon=[];
  var tempoff=[];
  var hum=[];
  var light2=[];
  var light3=[];
  for(var i=0;i!=data.length;i=i+31){
  labels.push(data.substr(i,5));
temperature1.push(data.substr(i+8,5));
hum.push(data.substr(i+13,5));
light.push(data.substr(i+18,4));
tempon.push(data.substr(i+26,2));


//light[i]=parseInt(light[i], 10)/10;
}

 for(var i=0;i!=light.length;i++){
 //console.log(light[i]);

 light2[i]=parseInt(light[i], 10);
light2[i]=light2[i]/1024.0;
light2[i]=5-light2[i]*5.0;
 //light[i]=parseInt(light[i], 10)/100;
 // light2[i]=1023-light[i]*100;
  

//light2[i]=500/(10.0*((5-light2[i])/light2[i])); 
 light[i]=((5-light2[i])/light2[i])/2;
hum[i]=parseFloat(hum[i], 10)/10;
tempoff[i]=parseInt(tempon[i], 10)+3;
//console.log(light[i]);
}
console.log(light2);
//console.log(tempon);

  res.write(`<!doctype html><html><head><meta charset="utf-8"><script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js"></script>


</head>
<body>
<canvas id="myChart" width="3500" height="800"></canvas>
<script>
var ctx = document.getElementById('myChart').getContext('2d');
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
  	showLine: false,

    labels: ${JSON.stringify(labels)},
    datasets: [{

fill: false,
borderColor: "rgba(255,0,0,1)",
      label: 'Температура °',
      data: ${JSON.stringify(temperature1)},
      backgroundColor: "rgba(255,0,0,1)"
    }, {
    	               label: 'Уровень включения насоса °',
    	               fill: false,
    	               borderColor: "rgba(255,255,0,1)",
      data: ${JSON.stringify(tempon)},
      backgroundColor: "rgba(255,255,0,1)"
    }, {
    	               label: 'Уровень выключения насоса °',
    	               fill: false,
    	               borderColor: "rgba(255,255,0,1)",
      data: ${JSON.stringify(tempoff)},
      backgroundColor: "rgba(255,255,0,1)"
    }, {
      label: 'Свет * 100',
      fill: false,
      borderColor: "rgba(0,255,0,1)",
      data: ${JSON.stringify(light)},
      backgroundColor: "rgba(0,255,0,1)"
    }, {
    	 label: 'Влажность*10 %',
    	 fill: false,
    	 borderColor:  "rgba(0,255,255,1)",
      data: ${JSON.stringify(hum)},
      backgroundColor: "rgba(0,255,255,1)"

    }],


  },
 options: {
 	scales: {
            yAxes: [{
                ticks: {
                    max: 5,
                    min: 0,
                    stepSize: 0.5
                }
            }]
        },
 	
 	animation:{
            animateScale:false
        },

tooltips:{
tooltipTemplate: "<%= value %>",
mode:'x-axis'
},

 	elements: {
 		line:{


 		},
                        point: {
                            
                            hitRadius:22,
                        }
                    },
                responsive: false,
                hoverMode: 'index',
                stacked: false,
                scales: {
                    yAxes: [{
                        type: "linear", // only linear but allow scale type registration. This allows extensions to exist solely for log scale for instance
                        display: true,
                        position: "left",
                        id: "y-axis-1",
                    }, {
                        type: "linear", // only linear but allow scale type registration. This allows extensions to exist solely for log scale for instance
                        display: true,
                        position: "right",
                        id: "y-axis-2",
                        // grid line settings
                        gridLines: {
                            drawOnChartArea: false, // only want the grid lines for one axis to show up
                        },
                    }],
                }
            }
});

</script>












</body>

</html>
`);
 res.end();
});




 
});






app.get('/table', function (req, res) {


var file='';
if(req.query.q)
	file=req.query.q
else
	file='01042017'
console.log('./data/'+file+'.txt');
fs.readFile('./data/'+file+'.txt', 'utf8', function (err,data) {
  if (err) {
    return console.log(err);
  }
  var labels =[];
  var temperature1 =[];
  var light=[];
  var tempon=[];
  var tempoff=[];
  var hum=[];
  for(var i=0;i!=data.length;i=i+31){
  labels.push(data.substr(i,5));
temperature1.push(data.substr(i+8,5));
hum.push(data.substr(i+13,5));
light.push(data.substr(i+18,4));
tempon.push(data.substr(i+26,2));

//console.log(light[2]);
//light[i]=parseInt(light[i], 10)/10;
}

 for(var i=0;i!=light.length;i++){
 //console.log(light[i]);
 light[i]=parseInt(light[i], 10)/100;

hum[i]=parseFloat(hum[i], 10)/10;
tempoff[i]=parseInt(tempon[i], 10)+2;
//console.log(light[i]);
}

res.write('<!doctype html><html><head><meta charset="utf-8"></head><body><table>');

 for(var i=0;i!=labels.length;i++){

res.write(`<tr>`);
res.write(`<td>${labels[i]}</td>`);
res.write(`<td>${temperature1[i]}</td>`);
res.write(`<td>${light[i]}</td>`);
res.write(`<td>${tempon[i]}</td>`);
res.write(`<td>${hum[i]}</td>`);
res.write(`</tr>`);
}

res.end('</table></body></html>');
});


});






app.listen(5070, function () {

});



