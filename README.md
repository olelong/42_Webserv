# Webserv

C++, HTML, CSS language

A trinome project with [Yoojeong Tak](https://github.com/yooyoo56) and [Waël Hazami](https://github.com/whazami).

</br>
<p align="left"> *************************** ENGLISH *************************** </p>

<body>

## The Project:
        
The goal of the project is to create a web server and to manage the HTTP requests and responses it receives from the client.
        
## Prerequisites
* A Unix-based operating system (for example: Linux, macOS)
* GNU Make
        
## How does it works: 
Clone the repository:

``` git clone "git@github.com:olelong/42_Webserv.git" repo_git_name ```

Enter in the folder:
``` cd repo_git_name ```

Run the ``` make ``` command to run the server.

Launch the executable ``` ./webserv ```.  

Launch your browser google chrome and then type the url: "127.0.0.1:8080"

Then let's go and try our web server!

</br></br>

### The project is divided into three parts:

1. Parsing the configuration file.
2. Create a web server.
3. Manage HTTP requests and responses.

</br></br>

### 1. Parsing the configuration file:

#### The config file 

We must take inspiration from the Nginx configuration files.

The configuration file allows you to retrieve all the necessary information to create our server. 

It contains mainly the server name, the cgi, the root, the port on which to listen, links to html pages ...

An example of a config file:

```cpp

server  {
        server_name yoo

        client_max_body_size 1000000

         cgi php-cgi_ubuntu 

        root www
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

        root www
        location  / { 

        }
}


```

#### The CGI, what is it ? 

(Common Gateway Interface) The CGI is a gateway between our server and another service. The other service in our case is the interpretation of PHP files.


</br></br>

### 2. Create a web server:

A website can only run if it is hosted on a server. 

If you run the server on 127.0.0.1:8080, the server will fetch the pages, display them in the browser.

Localhost means that the site is only accessible on the local machine.

To make our server work, we will use the poll() function which will allow the management between the sockets*. With poll(),

we can know with flags such as POLLIN and POLLOUT if we can read or write on a socket.

We have created a configuration file to test our server, which allows to test multi-servers with different server_names.

Server_name* is useful to know which server we are on but also we can change it in the /etc/hosts file:

![image](https://user-images.githubusercontent.com/58531906/188330200-c657237b-d621-4ccb-8c4a-080daabe51da.png)

Above, we could add in the hosts file, our tabby-website. This will allow us to access our site via tabby-website instead of 127.0.0.1 or localhost even if the other options are still possible.

#### Vocabulary:

*socket = a tunnel between two machines using a particular protocol.
In our case, it is the TCP/IP protocol. Our server has a socket and our clients have one too.

*server_name = name assigned to a server.

#### The different servers:

server_name : port
![image](https://user-images.githubusercontent.com/58531906/188330437-9aba6334-11ad-47e2-8b30-287b2a4e062f.png)

- Ports 8080 and 8081 test our website created by @yooyoo56.

![image](https://user-images.githubusercontent.com/58531906/188732314-9a47a265-9bcb-4107-9b14-9a69ac5da85c.png)
![image](https://user-images.githubusercontent.com/58531906/188732369-0b3125bc-4c8a-407e-9890-f3027b60959a.png)

- The port 8082 tests the autoindex, that is to say the fact of being able to access in a html page to all documents and files of the root.

![image](https://user-images.githubusercontent.com/58531906/188732028-6b12437f-1ae1-4d08-8655-2ed5e7e59508.png)

- Port 8083 checks if the GET method returns error_pages Method Not Allowed because it has not been added to the list of
accepted methods in the config file.

- Port 8084, 8085 and 8086 test our cgi. On port 8084, we can upload a file in cgi_website/cgi-bin/test.php and

if it is a .php file, compile it via cgi-php. The port 8085 allows to test to upload a file with a body superior to the max_body_size,
so this server returns on error_page Entity too large. Finally, the port 8086, allows to test when it is not possible to upload a file.

- The port 8087 tests the redirections by going to test_redir/redir_from/, we are redirected to /test_redir/redir_to/www/blabla/:

![image](https://user-images.githubusercontent.com/58531906/188330871-3edf4365-94a9-4aa4-84fc-97ab54f63f4e.png)

![image](https://user-images.githubusercontent.com/58531906/188330878-d811145e-2c13-4647-9a52-2bf58025206f.png)

- Port 8088 is an empty html page that allows us to stress test our server with siege:

##### Install siege:

```
mygit_webserv git:(main) ✗ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

eval "$(/mnt/nfs/homes/login/.linuxbrew/bin/brew shellenv)" => example of path to brew

brew install siege

```

##### Utilisation de siege:

```

siege -b http://localhost:8088

```

![image](https://user-images.githubusercontent.com/58531906/188331183-287e524c-2aec-490d-a1cb-9bfbccb849d8.png)


</br></br>



</br>

### 3. Manage the HTTP requests and responses:


##### The RFC, what is it ? 

We use RFC 2616 as a reference for the server status codes that will be sent in response to the client


#### What is a request?

When you are on a website and you click on a link on the page or try to do an action, 
it sends a request to the server. The server analyzes the client's request and then sends 
back a response so if the client wants to access the contact tab of the site for example, 
the server will send the client to this tab by sending back the link to the html page linked 
and if it fails it will send the client to an error page.

There are different methods used in requests, the 3 that we will use in our project are : 

- GET, allows you to request an html file, an image ...
- POST, allows you to modify data on the server.
- DELETE,  allows you to delete data from the server.

#### In our project, we handled the requests and responses in the following way:

The request is received end by end by the server then an append() function returns false until the request is

end of the request.

Once the request is received in full, it is parsed and analyzed to check for errors.

In the analysedReq structure is added all the useful elements to the server such as headers, file name ...

This structure is sent and used by the server so that it can later create the body of the response.

The response is created from the analysis of the request and the body given by the server.

This response is then sent to the server via the send() function.

#### Examples of HTTP requests

Exemple of a simple request:

![request](./img/request.png)

Exemple with the API of Spotify:

<img src="./img/SpotifyAPI1.png" width="400" height="550" />

The goal is to make a request to access a playlist.

So we will use the GET method: ``` GET https://api.spotify.com/v1/playlists/{playlist_id} ```


Request: 

```
curl -X "GET" "https://api.spotify.com/v1/playlists/0vvXsWCC9xrXsKd4FyS8kM" -H "Accept: application/json" -H "Content-Type: application/json" -H "Authorization: Bearer your_token_here"

```

![API2](./img/SpotifyAPI2.png)

Response (Cut because it is very long):

![API3](./img/SpotifyAPI3.png)

</br></br>

To conclude, the server receives requests from the client sockets. These requests are then analyzed and the response is
generated taking into account the elements present in the configuration file. And so our site is displayed on port 8080.

![image](https://user-images.githubusercontent.com/58531906/188732613-eea8ac1c-0ea6-4f6b-8f7a-0710ab503cd7.png)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

</br></br>
</br></br>
</body>

<p align="left"> *************************** FRANCAIS *************************** </p>

<body>
        
## Le projet :
        
Le but du projet est de créer un serveur web et de gérer les requêtes HTTP et les réponses qu'il reçoit 
du client.        
        
## Prérequis
* Un système d'exploitation basé sur Unix (par exemple: Linux, macOS)
* Un GNU Make
        
## Comment le faire fonctionner : 
        
Clonez le repository:
``` git clone "git@github.com:olelong/42_Webserv.git" repo_git_name ```

Entrez dans le dossier:        
``` cd repo_git_name ```

Exécutez la commande ``` make ``` pour lancer le serveur.

Lancez l'exécutable ``` ./webserv ```.  

Démarrez votre navigateur google chrome et ensuite tapez l'url : "127.0.0.1:8080"

Ensuite, allez-y maintenant vous pouvez tester notre serveur web !

</br>

### Le projet est divisé en trois parties :

1. Parser le fichier de configuration.
2. Créer un serveur web.
3. Gérer les requêtes et les réponses HTTP.

</br></br>


### 1. Parser le fichier de configuration :

#### Le fichier de configuration

Nous devons nous inspirer des fichiers de configuration de Nginx.

Un exemple de fichier de configuration :

Le fichier de configuration permet de récupérer toutes les informations nécessaires pour créer notre serveur. 

Il contient principalement le nom du serveur, le cgi, la racine, le port sur lequel écouter, des liens vers des pages html ...

```cpp

server  {
        server_name yoo

        client_max_body_size 1000000

         cgi php-cgi_ubuntu 

        root www
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

        root www
        location  / { 

        }
}

```

##### Le CGI, qu'est ce que c'est ? 

(Common Gateway Interface)
Le CGI est une passerelle entre notre serveur et un autre service. L'autre service dans notre cas est l'interprétation de fichiers PHP.

</br></br>

### 2. Créer un serveur web :

Un site Web peut tourner que s'íl est hébergé sur un serveur. 

Si l'on lance le serveur sur 127.0.0.1:8080, le serveur récupère les pages, les affichent via le navigateur. 

Localhost signifie que le site n'est accessible que sur la machine locale.

Pour faire fonctionner notre serveur, on va utiliser la fonction poll() qui permettra la gestion entre les *sockets. Avec poll(),

on peut savoir avec des flags tels que POLLIN et POLLOUT si l'on peut lire ou écrire sur une socket.

Nous avons créer un fichier de configuration pour tester notre serveur, qui permet de tester les 

multi-serveurs avec diffèrents server_names.

Server_name* est utile pour savoir sur quel serveur nous sommes mais aussi on peut changer dans 

le fichier /etc/hosts: 

![image](https://user-images.githubusercontent.com/58531906/188330200-c657237b-d621-4ccb-8c4a-080daabe51da.png)

Ci-dessus, on a pu rajouter dans le fichier hosts, notre tabby-website. Ce qui va nous permettre de pouvoir 

accéder a notre site via tabby-website au lieu de 127.0.0.1 ou de localhost même si les autres options sont toujours possibles.


#### Vocabulaire:

*socket = un tunnel entre deux machines utilisant un protocole en particulier. 
Dans notre cas, c'est le protocole TCP/IP. Notre serveur a une socket et nos clients en ont une aussi.

*server_name = nom attribué a un serveur.

#### Les différents serveurs:

server_name : port

![image](https://user-images.githubusercontent.com/58531906/188330437-9aba6334-11ad-47e2-8b30-287b2a4e062f.png)

- Les ports 8080 et 8081 testent notre website créé par @yooyoo56.

![image](https://user-images.githubusercontent.com/58531906/188732314-9a47a265-9bcb-4107-9b14-9a69ac5da85c.png)
![image](https://user-images.githubusercontent.com/58531906/188732369-0b3125bc-4c8a-407e-9890-f3027b60959a.png)

- Le port 8082 teste l'autoindex, c'est à dire le fait de pouvoir accéder dans une page html à tous les documents et fichiers de la root.

![image](https://user-images.githubusercontent.com/58531906/188732091-db0f3317-b6a3-41f3-b4b8-f54fc37460b4.png)

- Le port 8083 check si la méthode GET nous renvoie bien sur error_pages Method Not Allowed car elle n'a pas été rajouté dans la liste des 
méthodes acceptées dans le config file.

- Les port 8084, 8085 et 8086 testent notre cgi. Sur le port 8084, on peut upload un fichier dans cgi_website/cgi-bin/test.php et 

si c'est un .php le compiler via cgi-php. Le port 8085 permet de tester d'upload un fichier avec un body supérieur a la max_body_size,

donc ce serveur renvoie sur error_page Entity too large. Enfin, le port 8086, permet de tester lorsqu'il n'est pas possible d'upload un fichier.

- Le port 8087 teste les redirections en allant dans test_redir/redir_from/, on est redirigé vers /test_redir/redir_to/www/blabla/:
 
![image](https://user-images.githubusercontent.com/58531906/188330871-3edf4365-94a9-4aa4-84fc-97ab54f63f4e.png)

![image](https://user-images.githubusercontent.com/58531906/188330878-d811145e-2c13-4647-9a52-2bf58025206f.png)

- Le port 8088 est une page html vide qui nous permet de stress tester avec siege notre serveur.

##### Pour install siege:

```
mygit_webserv git:(main) ✗ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

eval "$(/mnt/nfs/homes/login/.linuxbrew/bin/brew shellenv)" => exemple de path vers brew

brew install siege

```

##### Utilisation de siege:

```

siege -b http://localhost:8088

```

![image](https://user-images.githubusercontent.com/58531906/188331183-287e524c-2aec-490d-a1cb-9bfbccb849d8.png)


</br></br>

### 3. Gérer les requêtes et les réponses HTTP :

##### La RFC, qu'est-ce que c'est ? 

Nous utilisons la RFC 2616 comme référence pour les codes de statut du serveur qui seront envoyés en réponse au client.

#### Une requête, qu'est-ce que c'est ?

Lorsque vous êtes sur un site web et que vous cliquez sur un lien sur la page ou que vous essayez d'effectuer une action, 

il envoie une requête au serveur. Le serveur analyse la requête du client et renvoie ensuite une réponse. 

Ainsi, si le client veut accéder à l'onglet "contact" du site, par exemple, 

le serveur renverra le client vers cet onglet en renvoyant le lien vers la page html correspondante. 

et s'il échoue, il enverra le client vers une page d'erreur.

Il existe différentes méthodes utilisées dans les requêtes, les 3 que nous allons utiliser dans notre projet sont : 

- GET, permet de demander un fichier html, une image ...
- POST, permet de modifier des données sur le serveur.
- DELETE, permet de supprimer des données sur le serveur.


#### Dans notre projet, nous nous sommes occupés des requêtes et réponses de la facon suivante:

La requête est reçue bout par bout par le serveur ensuite une fonction append() renvoie false tant que ce n'est pas la
 fin de la requête.

Une fois la requête reçu entièrement, elle est parsée et analysée pour vérifier qu'il n'y a aucune erreur.

Dans la structure analysedReq est ajoutée tous les élèments utiles au serveur tel que des headers, le nom du fichier ...

Cette structure est envoyée et utilisée par le serveur pour qu'il puisse par la suite créer le body de la réponse.

La réponse est créée à partir de l'analyse de la requête et le body donné par le serveur.

Cette réponse est ensuite envoyée au serveur via la fonction send().

</br>

#### Exemples de requêtes et réponses HTTP

Exemple de simple requête:

![request](./img/request.png)


Exemple avec l'API de Spotify :

<img src="./img/SpotifyAPI1.png" width="400" height="550" />

Le but est de faire une requête pour accéder à une playlist.

Nous allons donc utiliser la méthode GET : ``` GET https://api.spotify.com/v1/playlists/{playlist_id} ```

Requête : 

```
curl -X "GET" "https://api.spotify.com/v1/playlists/0vvXsWCC9xrXsKd4FyS8kM" -H "Accept: application/json" -H "Content-Type: application/json" -H "Authorization: Bearer your_token_here"

```

![API2](./img/SpotifyAPI2.png)


Réponse (Coupée car elle est vraiment longue):

![API3](./img/SpotifyAPI3.png)

</br></br>

Pour conclure, le serveur reçoit des requêtes depuis les sockets des clients. Ces requêtes sont ensuite analysées et la réponse est ainsi 

générées prenant en compte les éléments présents dans le fichier de configuration. Et ainsi notre site s'affiche sur le port 8081.

![image](https://user-images.githubusercontent.com/58531906/188732567-edb2ecfe-4ba8-4025-9e4d-1ef895ecb006.png)

## Licence

Ce projet est sous licence MIT - voir le fichier [LICENSE](LICENSE) pour plus de détails.

</br></br>
</br></br>

</body>
