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


#### 1. Parsing the configuration file:

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

- GET,
- POST, 
- DELETE



An exemple of HTTP request :

![request](./img/request.png)

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

Un exemple de requête HTTP :

![request](./img/request.png)

</br></br>
</br></br>

