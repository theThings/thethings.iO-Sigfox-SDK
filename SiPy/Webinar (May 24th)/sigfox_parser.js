

function main(params, callback){
  	var number = '';
  	for (var i = 0; i < params.data.length; i+=2) {
    	number += String.fromCharCode(parseInt(params.data[i]+params.data[i+1], 16));
    }

  	var temperature = parseInt(number, 10) / 100.0;

    var result = [
        {
            "key": "temperature",
            "value": temperature
        }
    ];


  	var threshold = 30.00;

 	if (temperature >= threshold) {

      var message = 'Alarm: Temperature above ' + threshold + ': ' + temperature + ' ºC';

      email({
        service : 'SendGrid',
        auth: {
          api_user: 'API_USER',
          api_key: 'API_KEY'
        }
      },{
        from: 'FROM_EMAIL',
        to: 'TO_EMAIL',
        subject : 'Temperature alarm',
        text : message
      });

      var telf = 'PHONE';

      var twilio = new Twilio(
        'TWILIO_KEY_1',
        'TWILIO_KEY_2'
      );

      /*
      twilio.sendMessage({
          to: telf, // Any number Twilio can deliver to
          from: 'TWILIO_PHONE', // A number you bought from Twilio and can use for outbound communication
          body: message // body of the SMS message
        },callback);
   */

      twilio.makeCall({
       to: telf, // Any number Twilio can deliver to
       from: 'TWILIO_PHONE', // A number you bought from Twilio and can use for outbound communication
       url: 'http://hidden-cove-2761.herokuapp.com/' // A URL that produces an XML document (TwiML) which contains instructions for the call
       }, callback);


    return callback(null, result);
   }
    else return callback(null, result);
}
