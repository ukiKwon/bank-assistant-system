/*
  *
  *******  part1.환경설정 *******
  *
*/
//1.1.설정-모듈 라이브러리
var express = require("express");//express
var app = express();
var request = require('request');//request
var port = process.env.PORT || 4000;
var cors = require('cors');//cross-browsing
// ***********************************************************
// mysql-booster 관리
// var MysqlPoolBooster = require('mysql-pool-booster');
// mysql = MysqlPoolBooster(mysql);
// // db-configuration 적용
// mysql.createPool(db_config);
// ***********************************************************
app.use(express.static(__dirname + '/public'));//express
app.use(express.json());
app.use(express.urlencoded({extended:false}));

app.set('views', __dirname + '/views');//ejs
app.set('view engine', 'ejs');
app.engine('html', require('ejs').renderFile);

app.use(cors());//cross-browsing
/*
  *
  *******  part2.프론트-화면 렌더링 *******
  *
*/
//2-1. 메인
app.get('/', function (req, res) {
    res.render('index')
})
//3-1.학교정보
app.post('/', function (req, res) {
    var getTokenUrl = "https://dev-openapi.kbstar.com:8443/land/getSchoolList/ver1";
    var option = {
        method : "POST",
        url : getTokenUrl,
        headers : {
            'Content-Type' : 'application/json; charset=UTF-8'
        },
        json : {
            bank_code_std : '097',
            account_num : "1234567890123456",
            account_holder_info_type : ' ',
            account_holder_info :  '911201',
            tran_dtime : '20160310101921'
        }
    };
    request(option, function(err, response, body){
      if(err) {throw err;}
      else {
        console.log(body);
        // res.json(body)
      }
    })
})
//3.$. 서버처리-대기
app.listen(4000);
console.log("Listening on port", port);
