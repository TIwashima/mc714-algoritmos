# MC714: Sistemas distribuídos - Prova 02

Demonstra o relógio de Lamport, o algoritmo de exclusão mútua e o algoritmo de eleição de líder

## Configurando as máquinas

### Grupo de segurança

Todas as instâncias precisam estar no mesmo grupo de segurança na AWS. Nesse grupo de segurança, devemos colocar tanto as Inbound Rules quanto as Outbound Rules com o tipo `All traffic` e a fonte `Anywhere`.

### Gerando chaves públicas

Ao criar as instâncias na AWS, devemos baixar um arquivo `chave.pem` que usaremos para acessar cada instância por ssh. Podemos gerar uma chave pública com o comando 

> openssl rsa -in chave.pem -pubout -out chave.pub

Essas chaves podem ser colocados na pasta `~/.ssh` em cada uma de nossas instâncias.

### Adicionando os hosts

Aqui usamos 3 máquinas. Uma deve ser escolhida como máquina principal, a partir da qual vamos executar o código. Nessa máquina, é preciso editar o arquivo `/etc/hosts`, adicionando o endereço das duas outras máquinas com os nomes `serv02` e `serv03`.

### Adicionando as chaves ssh

Em cada uma de nossas instâncias, devemos executar o seguinte comando:

> eval \`ssh-agent -s` && ssh-add ~/.ssh/chave.pem

Agora podemos acessar as máquinas de uma até a outra por ssh

## Instalação do OpenMPI

O OpenMPI pode ser instalado em cada máquina com os seguintes comandos:

```
wget https://download.open-mpi.org/release/open-mpi/v4.1/openmpi-4.1.2.tar.gz
tar -xzf openmpi-4.1.2.tar.gz
cd openmpi-4.1.2
./configure
make; sudo make install
```

## Executando o projeto

Para executar o projeto, deve-se estar dentro do diretório clonado e usar o comando `make run`. O Makefile irá compilar o código, enviar o executável para as outras duas máquinas e executá-lo. Ao fim, ele será deletado.
