Facebook provides a Rest API and the newer Graph API, which is the successor of the Rest API. While implementing the Facebook interface into Sally, we decided to use the new Graph API. More informations about the Graph API can be found here:

Wikipedia Page: http://de.wikipedia.org/wiki/Facebook#Open_Graph

Facebook API Doc: http://developers.facebook.com/docs/api


#### Getting the Access Token ####

First of all we need Sally to request an access token for the users Facebook account. This token can be used to get informations from Facebook about the user or to send data to Facebook. Every token is unique and identifies a specific user to application relationship. That ensures that the token can only be used together with the app which requested the token.

We are using the Facebook scope 'offline\_access' and 'publish\_stream'.
  * 'offline\_access' means Sally will get a token which can also be used when the user loges out in the web browser.
  * 'publish\_stream' gets rights to post a message to the user wall.



Here the single steps which are done to get the access token:

  * To get the access token, Sally will first generate a unique "Sally Key" for this machine.
  * Than Sally opens a browser window with an URL to the Facebook server. This URL includes the following parameters
    * redirect\_uri: A URL the user is redirect to when he gave his allowance that Sally can interact with Facebook. This URL also includes the unique "Sally Key" generated on the client.
    * client\_id: The Facebook id for the Sally application
    * scope: offline\_access,publish\_stream (see above)
  * On the Facebook page the user will need to login and "Allow" Sally to interact with the users Facebook profile.
  * After giving the allowance, the user will be redirected to a page send in the parameter "redirect\_uri". This will bring the user to http://www.sally-project.org/facebookAccess.php .
  * Here the php script fetches the access token from the Facebook server by sending the "client\_secret" (post to the php page) and some other variables like client\_id.
  * Now we have the "Sally Key" (part of the URL Facebook redirected the user to) and the access key. This data will be stored in the database - finally :D We only need to bring the access token to the Sally client ;)
  * Sally itself will query the homepage sending the "Sally Key". This will be done in a loop all 5 seconds. The loop will end when:
    * Either the server sends back the access token - the server searches in the database for the same "Sally Key" send from the client and if he has a access token for tis key
    * Or the user clicks in the Sally 'Facebook Connection' dialog the "Cancel" button


<font color='#22AA22'>
<h2>Info</h2>
<blockquote>When the php script saves the access token together with the "Sally Key", it performs an update of the user informations.<br>
</font></blockquote>


#### Posting to the Facebook wall ####
--coming soon--


#### Update Facebook Informations ####
--coming soon--


#### Getting Friend Status updates ####
--coming soon--