# Webserv

C++, HTML, CSS language

A trinome project with @ytak and @whazami.

The goal of the project is to create a web server and to manage the HTTP requests and responses it receives from the client.

</br>
<p align="left"> **************************************************** ENGLISH ******************************************************* </p>

### How to make the exercises work: 

``` git clone "git@github.com:olelong/42_Webserv.git" repo_git_name ```

``` cd repo_git_name ```

Run the ``` make ``` command to run the server.

Launch the executable ``` ./webserv ```.  

Launch your browser google chrome and then type the url: "127.0.0.1:8080"

Then let's go and try our web server!

</br>

### The project is divided into three parts:

#####        1. Parsing the configuration file.
#####        2. Create a web server
#####        3. Manage HTTP requests and responses

#### 1. Parsing the configuration file:

##### The config file 

We must take inspiration from the Nginx configuration files.

The configuration file allows you to retrieve all the necessary information to create our server. 

It contains mainly the server name, the cgi, the root, the port on which to listen, links to html pages ...

An example of a config file:

```cpp

server  {
        server_name yooyoo

        client_max_body_size 1000000

         cgi .yooyoo_php php-cgi_ubuntu 

        root http
        listen 1232

    error_page 404 my_errors/404.html

        location /data {  
        root www/second_data
        index fifth_index.html
                http_accept GET POST DELETE
   } 
}

server {
        server_name tabby
        listen 8080

        root re
        location  / { 

        }
}


```

##### The CGI, what is it ? 

(Common Gateway Interface), is an interface that allows you to tell the compiler 

how to compile certain file extensions like files.py, .php ...


#### 2. Create a web server:
#### 3. Manage the HTTP requests and responses:


##### The RFC, what is it ? 

We use RFC 2616 as a reference for the server status codes that will be sent in response to the client


##### What is a request?

When you are on a website and you click on a link on the page or try to do an action, 

it sends a request to the server. The server analyzes the client's request and then sends 

back a response so if the client wants to access the contact tab of the site for example, 

the server will send the client to this tab by sending back the link to the html page linked 

and if it fails it will send the client to an error page.

There are different methods used in requests, the 3 that we will use in our project are : 

- GET, allows you to request an html file, an image ...
- POST, allows you to modify data on the server.
- DELETE,  allows you to delete data from the server.

##### Examples of HTTP requests

Exemple of a simple request:

![request](./img/request.png)

Exemple with the API of Spotify:

<img src="./img/SpotifyAPI1.png" width="400" height="550" />

The goal is to make a request to access a playlist.

So we will use the GET method: ``` GET https://api.spotify.com/v1/playlists/{playlist_id} ```


Request: 

```
curl -X "GET" "https://api.spotify.com/v1/playlists/0vvXsWCC9xrXsKd4FyS8kM" -H "Accept: application/json" -H "Content-Type: application/json" -H "Authorization: Bearer BQD2O2p5T9DQDVNSF0WJzmaT8IbvnJUxoGjTvSPp67f4WF0YZ2U2fpGScrX8AImvKW1EJ8djPaD-XqKtQpnDMxWg-LRK50YM2MdeTNcpYASqQsVc5jppYBkHKGqK2QELu2RND2J8k21OBqgClgNCwSKOgJMzcsmR17P8C1HA5jOuktySV8XodxPgxtAyKGa5u14zL0fanSv4J1Sy88UNxbsDlaiEj0iPfGC0Sm-HWUN5wLMLfXVB"

```

![API2](./img/SpotifyAPI2.png)

Response (Cut because it is very long):

![API3](./img/SpotifyAPI3.png)


</br></br>
</br></br>

<p align="left"> **************************************************** FRANÇAIS ******************************************************* </p>

### Comment faire fonctionner les exercices : 

``` git clone "git@github.com:olelong/42_Webserv.git" repo_git_name ```

``` cd repo_git_name ```

Exécutez la commande ``` make `` pour lancer le serveur.

Lancez l'exécutable `` ./webserv ``.  

Démarrez votre navigateur google chrome et ensuite tapez l'url : "127.0.0.1:8080"

Ensuite, allez-y maintenant vous pouvez tester notre serveur web !

</br>

### Le projet est divisé en trois parties :


#### 1. Parser le fichier de configuration :

##### Le fichier de configuration

Nous devons nous inspirer des fichiers de configuration de Nginx.

Un exemple de fichier de configuration :

Le fichier de configuration permet de récupérer toutes les informations nécessaires pour créer notre serveur. 

Il contient principalement le nom du serveur, le cgi, la racine, le port sur lequel écouter, des liens vers des pages html ...

```cpp

server  {
        server_name yooyoo

        client_max_body_size 1000000

         cgi .yooyoo_php php-cgi_ubuntu 

        root http
        listen 1232

    error_page 404 my_errors/404.html

        location /data {  
        root www/second_data
        index fifth_index.html
                http_accept GET POST DELETE
   } 
}

server {
        server_name tabby
        listen 8080

        root re
        location  / { 

        }
}

```

##### Le CGI, qu'est ce que c'est ? 

(Common Gateway Interface), est une interface qui vous permet d'indiquer au compilateur 

comment compiler certaines extensions de fichiers comme les fichiers.py, .php ...

#### 2. Créer un serveur web :
#### 3. Gérer les requêtes et les réponses HTTP :

##### La RFC, qu'est-ce que c'est ? 

Nous utilisons la RFC 2616 comme référence pour les codes de statut du serveur qui seront envoyés en réponse au client.

##### Une requête, qu'est-ce que c'est ?

Lorsque vous êtes sur un site web et que vous cliquez sur un lien sur la page ou que vous essayez d'effectuer une action, 

il envoie une requête au serveur. Le serveur analyse la requête du client et renvoie ensuite une réponse. 

Ainsi, si le client veut accéder à l'onglet "contact" du site, par exemple, 

le serveur renverra le client vers cet onglet en renvoyant le lien vers la page html correspondante. 

et s'il échoue, il enverra le client vers une page d'erreur.

Il existe différentes méthodes utilisées dans les requêtes, les 3 que nous allons utiliser dans notre projet sont : 

- GET, permet de demander un fichier html, une image ...
- POST, permet de modifier des données sur le serveur.
- DELETE, permet de supprimer des données sur le serveur.

##### Exemples de requêtes et réponses HTTP

Exemple de simple requête:

![request](./img/request.png)


Exemple avec l'API de Spotify :

<img src="./img/SpotifyAPI1.png" width="400" height="550" />

Le but est de faire une requête pour accéder à une playlist.

Nous allons donc utiliser la méthode GET : ``` GET https://api.spotify.com/v1/playlists/{playlist_id} ```

Requête : 

```
curl -X "GET" "https://api.spotify.com/v1/playlists/0vvXsWCC9xrXsKd4FyS8kM" -H "Accept: application/json" -H "Content-Type: application/json" -H "Authorization: Bearer BQD2O2p5T9DQDVNSF0WJzmaT8IbvnJUxoGjTvSPp67f4WF0YZ2U2fpGScrX8AImvKW1EJ8djPaD-XqKtQpnDMxWg-LRK50YM2MdeTNcpYASqQsVc5jppYBkHKGqK2QELu2RND2J8k21OBqgClgNCwSKOgJMzcsmR17P8C1HA5jOuktySV8XodxPgxtAyKGa5u14zL0fanSv4J1Sy88UNxbsDlaiEj0iPfGC0Sm-HWUN5wLMLfXVB"

```

![API2](./img/SpotifyAPI2.png)


Réponse (Coupée car elle est vraiment longue):

![API3](./img/SpotifyAPI3.png)



</br></br>
</br></br>

