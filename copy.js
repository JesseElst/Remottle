var mqtt = require('mqtt');
var Topic = 'public/i363602_water/tilt/'; //subscribe to all topics

var Broker_URL = 'mqtt://mqtt.fhict.nl';
var Database_URL = 'localhost';

var options = {
	clientId: 'i363602_water',
	port: 1883,
	username: 'i363602_water',
	password: 'Dwa7tsgZm5NV7B',	
	keepalive : 60
};

var client  = mqtt.connect(Broker_URL, options);
client.on('connect', mqtt_connect);
client.on('reconnect', mqtt_reconnect);
client.on('error', mqtt_error);
client.on('message', mqtt_messsageReceived);
client.on('close', mqtt_close);

function mqtt_connect() {
    console.log("Connecting MQTT");
    client.subscribe(Topic, mqtt_subscribe);
}

function mqtt_subscribe(err, granted) {
    console.log("Subscribed to " + Topic);
    if (err) {console.log(err);}
}

function mqtt_reconnect(err) {
    console.log("Reconnect MQTT");
    if (err) {console.log(err);}
	client  = mqtt.connect(Broker_URL, options);
}

function mqtt_error(err) {
    console.log("Error!");
	if (err) {console.log(err);}
}

function mqtt_messsageReceived(topic, message, packet) {
	console.log('Message received = ' + message);
    console.log('test');
	insert_message(topic, message, packet);
}

function mqtt_close() {
	console.log("Close MQTT");
}
////////////////////////////////////////////////////
///////////////////// MYSQL ////////////////////////
////////////////////////////////////////////////////
var mysql = require('mysql');

//Create Connection
var connection = mysql.createConnection({
	host: Database_URL,
	user: "root",
	password: "",
	database: "water"
});

connection.connect(function(err) {
	if (err) throw err;
	console.log("Database Connected!");
});

//insert a row into the tbl_messages table
function insert_message(topic, message, packet) {
    
	var clientID= "client001";
    var d = new Date();
    var dag = d.getDate();
    var maand = d.getMonth() +1 ;
    var jaar = d.getFullYear();
    var volledig = dag+ "" + maand + "" + jaar;
    console.log(volledig);
	var sql = "INSERT INTO fles (water) VALUES ('" + message + "')";
	var params = ['tbl_messages', 'clientID', 'topic', 'message', clientID, topic, message];
	sql = mysql.format(sql, params);	
	connection.query(sql, function (error, results) {
		if (error) throw error;
		console.log("1 record inserted");
	});
};		
 