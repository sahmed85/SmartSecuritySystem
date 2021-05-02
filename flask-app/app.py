#!flask/bin/python
import sys, os
sys.path.append(os.path.abspath(os.path.join('..', 'utils')))
from env import *
from flask import Flask, jsonify, abort, request, make_response, url_for
from flask import render_template, redirect
from flask import session, escape
import time
import json
import boto3
from boto3.dynamodb.conditions import Key, Attr
from boto3.dynamodb.types import Binary
from botocore.exceptions import ClientError
from datetime import datetime, timedelta
import pytz
from pytz import timezone



app = Flask(__name__, static_url_path="")
server_hostname = SERVER_HOSTNAME

dynamodb = boto3.resource('dynamodb', aws_access_key_id=AWS_ACCESS_KEY,
                            aws_secret_access_key=AWS_SECRET_ACCESS_KEY,
                            region_name=AWS_REGION)

security_unlock_events = dynamodb.Table("SmartSecurityUnlockEvents")
security_video_events = dynamodb.Table("SmartSecurityVideoEvents")

ALLOWED_EXTENSIONS = set(['png', 'jpg', 'jpeg'])


###############################################################################################################
# Application Handling

@app.errorhandler(400)
def bad_request(error):
    """ 400 page route.

    get:
        description: Endpoint to return a bad request 400 page.
        responses: Returns 400 object.
    """
    return make_response(jsonify({'error': 'This feature has not been built yet!'}), 400)



@app.errorhandler(404)
def not_found(error):
    """ 404 page route.

    get:
        description: Endpoint to return a not found 404 page.
        responses: Returns 404 object.
    """
    return make_response(jsonify({'error': 'This feature has not been built yet!'}), 404)


# app route for the home page
@app.route('/', methods=['GET','POST'])
def login_page():
    # Login Page route
    if request.method == 'POST':
        #handle the login parameters
        # user_email = request.form['username']
        # user_password = request.form['password']
        # if(validate_user(user_email,user_password)):
        #     session['email'] = user_email
        return redirect(url_for('home_page'))
    else:
        #return the login page temple
        return render_template('login.html', login_status = 'Please login using your email and password.')


# app route for home page -- the unlock events page
@app.route('/home', methods=['GET','POST'])
def home_page():
    # scan all unlock in the DB
    unlock_response = security_unlock_events.scan()
    # sanitize the data into list to pass into template to render
    timestamp_list = []
    # loop through all entries in the table
    for item in unlock_response['Items']:
        timestamp_list.append(datetime.fromtimestamp(int(item['timestamp'])/1000))
    # order the timestamp and convert into Date and Time
    timestamp_list.sort()
    all_time_event = []
    for item in timestamp_list:
        this_event = {}
        this_event["day"]= item.strftime("%d")
        this_event["month"]= item.strftime("%b")
        this_event["year"]= item.strftime("%Y")
        this_event["hour"]= item.strftime("%H")
        this_event["min"]= item.strftime("%M")
        this_event["sec"]= item.strftime("%S")
        all_time_event.append(this_event)
    print(all_time_event)
    # pass this into the unlock events dashboard
    return render_template('unlock_events.html', data = all_time_event)
    



# app route for video dashboard page
@app.route('/motionDetecion', methods=['GET'])
def video_page():
    # scan all motion detection events in the DB
    video_response = security_video_events.scan()
    # sanitize the data into list to pass into template to render
    timestamp_list = []
    video_links = []
    # loop through all entries in the table
    for item in video_response['Items']:
        timestamp_list.append(datetime.fromtimestamp(int(item['timestamp'])/1000))
        video_links.append('https://smartsecuritysystemvideos.s3.amazonaws.com/home/pi/VideoCapture/MotionVideoCapture/detections/' + item['timestamp'] + '.mp4')
    # order the timestamp and convert into Date and Time
    sorted_table_data = sorted(zip(timestamp_list,video_links))
    timestamp_sorted = [x[0] for x in sorted_table_data]
    video_links_sorted = [x[1] for x in sorted_table_data]
    all_time_event = []
    index = 0
    for item in timestamp_sorted:
        this_event = {}
        this_event["day"]= item.strftime("%d")
        this_event["month"]= item.strftime("%b")
        this_event["year"]= item.strftime("%Y")
        this_event["hour"]= item.strftime("%H")
        this_event["min"]= item.strftime("%M")
        this_event["sec"]= item.strftime("%S")
        this_event["link"] = video_links_sorted[index]
        index = index + 1
        all_time_event.append(this_event)
    print(all_time_event)
    # pass this into the unlock events dashboard
    return render_template('video_events.html', data = all_time_event)


# # app route for confirmation from email
# @app.route('/confirm/<string:tokenID>', methods=['GET'])
# def confirm_page(tokenID):
#     #GET request with the token passed in needs to be confirmed
#     user_token = tokenID
#     if(check_confirmToken(user_token)):
#         # If token is correct and table is updated then redirect user to login page
#         return render_template('login.html', login_status = "Account confirmed! Please login using your email and password.")
#     else:
#         abort(404)


if __name__ == '__main__':
    app.run(debug=True, host="127.0.0.1", port=5000)
