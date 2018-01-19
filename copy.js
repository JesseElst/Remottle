var mqtt = require('mqtt'); 
var Topic = 'public/i363602_water/tilt/'; // subscribe op deze topic
var Broker_URL = 'mqtt://mqtt.fhict.nl'; //zoek op deze url
var Database_URL = 'localhost';//zoek database op de localhost

//voer gegevens in om in te loggen
var options = {
	clientId: 'i363602_water',
	port: 1883,
	username: 'i363602_water',
	password: 'Dwa7tsgZm5NV7B',	
	keepalive : 60
};

// maak verbinding om de handelingen uit te voeren
var client  = mqtt.connect(Broker_URL, options);
client.on('connect', mqtt_connect);
client.on('reconnect', mqtt_reconnect);
client.on('error', mqtt_error);
client.on('message', mqtt_messsageReceived);
client.on('close', mqtt_close);

//maakt verbinding met MQTT
function mqtt_connect() {
    console.log("Connecting MQTT");
    client.subscribe(Topic, mqtt_subscribe);
}

//subsribe op de topic
function mqtt_subscribe(err, granted) {
    console.log("Subscribed to " + Topic);
    if (err) {console.log(err);}
}

//maak opnieuw verbinding 
function mqtt_reconnect(err) {
    console.log("Reconnect MQTT");
    if (err) {console.log(err);}
	client  = mqtt.connect(Broker_URL, options);
}

//geef een error als er een fout is
function mqtt_error(err) {
    console.log("Error!");
	if (err) {console.log(err);}
}

//stuur het ontvangen bericht naar een database
function mqtt_messsageReceived(message) {
	console.log('Message received = ' + message);
    console.log('test');
	insert_message(message);
}

//stop verbinding MQTT
function mqtt_close() {
	console.log("Close MQTT");
}




///////////////////// MYSQL ////////////////////////


var mysql = require('mysql');

//maak verbinding met database
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

//voeg het bericht toe aan de database
function insert_message(message) {
    //laat zien welke dag het is
    var d = new Date();
    var dag = d.getDate();
    var maand = d.getMonth() +1 ;
    var jaar = d.getFullYear();
    var volledig = dag+ "" + maand + "" + jaar;
    console.log(volledig);
    //voorgedefinieerd statement om sql injection tegen te gaan
	var sql = "INSERT INTO ?? (??) VALUES (??)";
    //waardes die in de sql code ingevoerd worden
	var params = ['fles', 'water', message];
	sql = mysql.format(sql, params);	
	connection.query(sql, function (error, results) {
		if (error) throw error;
		console.log("1 record inserted");
	});
};		
 