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


The configuration file allows you to retrieve all the necessary information to

create our server.

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

#### 2. Create a web server:
#### 3. Manage the HTTP requests and responses:

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

Le fichier de configuration permet de récupérer toutes les informations nécessaires pour

créer notre serveur.

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

#### 2. Créer un serveur web :
#### 3. Gérer les requêtes et les réponses HTTP :

Un exemple de requête HTTP :

![request](./img/request.png)

</br></br>
</br></br>

