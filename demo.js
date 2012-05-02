var ictclas = require('./build/Release/ictclas');
var s = "反省中：在餐馆吃饭，等米饭太久，失去耐心，对服务员大吼，瞬间变成我鄙视的人";
ictclas.init('/opt/ictcla/Sample/Linux_64_C_Demo');
ictclas.importDictFile("/ws/ictclas/userdict.txt");
ictclas.segment(s, function(msg){
	console.log(msg);
});