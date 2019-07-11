/*
  *
  *******  part1.환경설정 *******
  *
*/
//1.1.설정-모듈 라이브러리
var express = require("express");//express
var app = express();
var request = require("request");//request
var port = process.env.PORT || 3000;

// ***********************************************************
//1.2.설정-연동

app.use(express.static(__dirname + '/public'));//express
app.use(express.json());
app.use(express.urlencoded({extended:false}));

app.set('views', __dirname + '/views');//ejs
app.set('view engine', 'ejs');
app.engine('html', require('ejs').renderFile);

/*
  *
  *******  part2.프론트-화면 렌더링 *******
  *
*/
//2-1. 메인
app.get('/', function (req, res) {
    res.render('index')
})
//2-2. 정적인 XY 뷰
app.get('/static_xy_point', function (req, res) {
    res.render('static_xy_point')
})
//3.$. 서버처리-대기
app.listen(3000);
console.log("Listening on port", port);
