

# ComplexCi

Este proyecto (ComplexCi) se centra principalmente en la implementación en C++ del algoritmo de Influencia Colectiva (CI), diseñado para encontrar el nodo más importante (o influenciadores) en la Red Compleja mediante la percolación óptima desarrollada por:

> Morone F, Makse H A. Influence maximization in complex networks through optimal percolation[J]. Nature, 2015, 524(7563): 65-U122. 

> Morone F, Min B, Bo L, et al. Collective Influence Algorithm to find influencers via optimal percolation in massively large social media[J]. Scientific reports, 2016, 6.  

Este proyecto también cubre el nuevo algoritmo CI_DR propuesto en https://www.nature.com/articles/s41598-018-32874-5 .

> Fengkuangtian Zhu. Improved collective influence of finding most influential nodes based on disjoint-set reinsertion[J]. Scientific reports, 2018, 9.  

Los usuarios pueden usar el script `newReinsertCollectiveInfluence` en este proyecto para probar el CI_DR.
  

## Algoritmo  
  
En general, el objetivo del algoritmo CI es proporcionar una lista de clasificación de nodos según su importancia, y los nodos mejor clasificados tendrán mayor importancia. Podemos eliminar los nodos desde los mejor clasificados en la lista generada por el algoritmo CI y calcular el tamaño del componente gigante después de cada eliminación, lo que descompondrá la red en muchas piezas desconectadas. La proporción del componente gigante alcanzará cero con la operación de eliminación uno por uno finalmente final. Por lo tanto, cuanto mejor sea el algoritmo, antes colapsará la red a un componente gigante cero con un número menor de nodos proporcionados.

Los usuarios pueden obtener el conjunto mínimo de influenciadores de la Red Compleja mediante el programa en C++ de este repositorio . Dado que los algoritmos CI pueden utilizarse en tantos campos científicos, implementé este algoritmo en código de estilo moderno de C++. En comparación con el código C original http://www-levich.engr.ccny.cuny.edu/~hernanlab/uploads/CI_HEAP.c mencionado en http://www-levich.engr.ccny.cuny.edu/webpage/hmakse/software-and-data/ junto con el artículo anterior, ComplexCi tiene las siguientes características :

* Este proyecto utiliza el estilo de código C++ de programación orientada a objetos moderna con la Biblioteca de Plantillas Estándar (STL), lo que hace que sea más fácil para los usuarios modificarlo que el código C original. Tras revisar el código original en C `CI_HEAP.c`, creo que es difícil para los usuarios leerlo, mantenerlo e implementarlo si tienen sus propias ideas, especialmente porque `CI_HEAP.c` contiene muchas abreviaturas de variables simples, punteros multinivel, gestión de memoria insegura y código C poco amigable.
* ComplexCi acepta más parámetros de entrada y pueden utilizarse para un comportamiento más flexible en la operación de nodos en la red proporcionada. 
  * Los usuarios pueden determinar el tamaño del lote de eliminación de nodos en ComplexCi por actualización de los valores CI cuando la Red Compleja colapsa
  * Los usuarios pueden determinar la proporción del componente gigante al iniciar el algoritmo de reinserción al que colapsa la red compleja
  * Los usuarios pueden determinar si necesitan la operación de reinserción en el algoritmo CI para optimizar el resultado
* Verificado que el resultado de la implementación C tradicional y la nueva C++ pueden alcanzar casi el mismo resultado en la métrica de Robustez, incluso la nueva implementación C++ es más eficiente y tarda mucho menos tiempo en algunos conjuntos de datos que el programa C tradicional. Ver en la Sección de Benchmark
* El `CI_HEAP.c` tradicional también se ha fusionado en este proyecto. Los usuarios pueden cambiar la opción para usar el nuevo diseñador C++ o el código de estilo C tradicional según sus preferencias.

## Uso

Esta sección describe el uso de ComplexCi y sus scripts correspondientes

### Obtener Repositorio

#### Descargar Versión

El archivo binario compilado basado en Windows 7 x64 se proporciona en https://github.com/zhfkt/ComplexCi/releases

#### O Obtener desde GitHub y Compilar

Los usuarios pueden clonar el repositorio directamente mediante el comando git o simplemente descargar el archivo ZIP desde la página web

```bash
git clone https://github.com/zhfkt/ComplexCi.git
```
El código contiene muchas características y sintaxis de C++11, por lo que el compilador de C++ debe soportar el estándar C++11. De hecho, solo se necesita compilar un archivo cpp: `ComplexCi/ComplexCi.cpp`, y no depende de ninguna biblioteca externa adicional.

* Linux

Los usuarios pueden entrar en la carpeta “bin/” bajo la carpeta raíz del proyecto y ejecutar 

```bash
g++ -pthread  ../ComplexCi/ComplexCi.cpp  -o ComplexCi -O3 -std=c++11 
```

o simplemente ejecutar

```bash
./make.sh
```

para generar el binario `bin/ComplexCi`. Tenga en cuenta que la versión de g++ debe soportar C++11. Para mi propio desarrollo, la compilación se realizó con éxito con g++ 5.4.0 en Ubuntu 16.04. De lo contrario, obtendrá fallos por varios errores de sintaxis incorrectos.

* Windows

Ya existe un archivo binario de Windows `x64/Release/ComplexCi.exe` compilado en Windows 7 x64 en el repositorio. Puede usarse directamente si los usuarios no necesitan recompilar ni modificar el código. Si los usuarios desean compilarlo ellos mismos y tienen Visual Studio 2013 o superior, pueden abrir directamente `ComplexCi.sln` y compilar el código en el IDE. El archivo binario se generará en `x64/Release/ComplexCi.exe`. Creo que los usuarios también pueden elegir cualquier otro IDE o compilador que soporte C++11 para compilar el archivo `ComplexCi/ComplexCi.cpp`. Tenga en cuenta que los usuarios deben reemplazar `x64/Release/ComplexCi.exe` con su propio archivo generado después de la compilación.


### Ejecutar

#### Formato del Archivo de Entrada de Red

Los datos de la red pueden escribirse en archivos .txt o .csv, y la red se considera como una red no dirigida. Cada fila contiene 2 identificadores de nodo divididos por una coma, lo que significa que hay una conexión entre estos 2 nodos. Los identificadores de nodo deben ser enteros y comenzar desde 0. Por ejemplo:

```
0,1
1,2
2,3
3,4
3,5
...
```

El ejemplo completo se encuentra en `data/test/karate.csv`

#### Inicio Rápido para Usar Scripts

Usar un script en el proyecto es una manera rápida de utilizar el algoritmo de Influencia Colectiva (CI). Hay 3 scripts que los usuarios pueden ejecutar en el proyecto. El proyecto contiene scripts bash para Linux y scripts cmd para Windows. Bash para Linux se puede encontrar en `dailyUse\linux` y cmd para Windows en `dailyUse\windows`.

* `traditionalCollectiveInfluence`

Los usuarios pueden usar este script para utilizar el código de estilo C tradicional de http://www-levich.engr.ccny.cuny.edu/~hernanlab/uploads/CI_HEAP.c. Lo he fusionado en este repositorio y configurado la capa de interfaz compatible para llamar al código C desde C++.

El script “traditionalCollectiveInfluence” acepta 3 parámetros:

```bash
./traditionalCollectiveInfluence.sh <networkPath> <ballRadius> <isPrintMinPointCausingMinComponent>


# "networkPath" es la ruta del archivo. El formato del archivo se describe en la sección Formato del Archivo de Entrada de Red
# "ballRadius" es el parámetro de Radio definido en el Algoritmo de Influencia Colectiva. Cuando ballRadius es cero, tenga en cuenta que el Algoritmo de Influencia Colectiva degenerará en el algoritmo HDA (High Degree Adaptive). es decir, el valor CI de cada nodo será igual al grado del nodo.
# "isPrintMinPointCausingMinComponent" indica si el archivo de salida contiene un punto limitado que conduce a una proporción de componente gigante del 0.01 o todos los puntos. Si se establece en 0, el programa generará todos los nodos. De lo contrario, el programa generará puntos parciales, lo que hará que la proporción del componente gigante sea 0.01 en el proceso de eliminación de nodos

```

Ejemplo: 

```bash
./traditionalCollectiveInfluence.sh /home/network/model1.csv 3 1
```

Esto significa que los usuarios están utilizando la estrategia `traditionalCollectiveInfluence` para el archivo de entrada `model1.csv` con el parámetro `ballRadius` 3. El archivo de salida solo contendrá los puntos parciales, lo que hará que la proporción del componente gigante sea 0.01 en el proceso de eliminación de nodos.


* `cppCollectiveInfluence`

Los usuarios pueden usar este script para utilizar la nueva implementación C++ del algoritmo de Influencia Colectiva (CI).

El script “cppCollectiveInfluence” acepta 4 parámetros:

```bash
./cppCollectiveInfluence.sh <networkPath> <ballRadius> <updateBatch> <isPrintMinPointCausingMinComponent>


# "networkPath" es la ruta del archivo. El formato del archivo se describe en la sección Formato del Archivo de Entrada de Red
# "ballRadius" es el parámetro de Radio definido en el Algoritmo de Influencia Colectiva. Cuando ballRadius es cero, tenga en cuenta que el Algoritmo de Influencia Colectiva degenerará en el algoritmo HDA (High Degree Adaptive). es decir, el valor CI de cada nodo será igual al grado del nodo.
# "updateBatch" tamaño del lote de eliminación de nodos en la Red por cada actualización de los valores CI cuando la Red Compleja colapsa
# "isPrintMinPointCausingMinComponent" indica si el archivo de salida contiene un punto limitado que conduce a 0 de proporción del componente gigante o todos los puntos. Si se establece en 0, el programa generará todos los nodos. De lo contrario, el programa generará puntos parciales, lo que hará que la proporción del componente gigante sea 0 en el proceso de eliminación de nodos. es decir, no quedan aristas pero aún quedan puntos en la red. Tenga en cuenta el comportamiento diferente en comparación con `traditionalCollectiveInfluence`. 

```

Ejemplo: 

```bash
./cppCollectiveInfluence.sh /home/network/model1.csv 2 500 0
```

Esto significa que los usuarios están utilizando la estrategia `cppCollectiveInfluence` para el archivo de entrada `model1.csv` con el parámetro `ballRadius` 2. El archivo de salida contendrá todos los nodos y 500 nodos se eliminarán en un lote por cada actualización de los valores CI cuando la Red Compleja colapsa.

* `newReinsertCollectiveInfluence` (CI_DR)

Este script incluye el nuevo algoritmo de reinserción de influencia colectiva (CI_DR). Tras verificarlo en 8 conjuntos de datos, este nuevo algoritmo puede lograr un mejor rendimiento en la métrica de Robustez que el algoritmo original de reinserción en influencia colectiva. Ver en las secciones de benchmark.

El CI_DR se propuso en https://www.nature.com/articles/s41598-018-32874-5 .

> Fengkuangtian Zhu. Improved collective influence of finding most influential nodes based on disjoint-set reinsertion[J]. Scientific reports, 2018, 9.  

El script “newReinsertCollectiveInfluence” acepta 4 parámetros:

```bash
./newReinsertCollectiveInfluence.sh <networkPath> <ballRadius> <updateBatch> <isPrintMinPointCausingMinComponent>

# "networkPath" es la ruta del archivo. El formato del archivo se describe en la sección Formato del Archivo de Entrada de Red
# "ballRadius" es el parámetro de Radio definido en el Algoritmo de Influencia Colectiva. Cuando ballRadius es cero, tenga en cuenta que el Algoritmo de Influencia Colectiva degenerará en el algoritmo HDA (High Degree Adaptive). es decir, el valor CI de cada nodo será igual al grado del nodo.
# "updateBatch" tamaño del lote de eliminación de nodos en la Red por cada actualización de los valores CI cuando la Red Compleja colapsa
# "isPrintMinPointCausingMinComponent" indica si el archivo de salida contiene un punto limitado que conduce a 0 de proporción del componente gigante o todos los puntos. Si se establece en 0, el programa generará todos los nodos. De lo contrario, el programa generará puntos parciales, lo que hará que la proporción del componente gigante sea 0 en el proceso de eliminación de nodos. es decir, no quedan aristas pero aún quedan puntos en la red. Tenga en cuenta el comportamiento diferente en comparación con `traditionalCollectiveInfluence`. 

```

Ejemplo: 

```bash
./newReinsertCollectiveInfluence.sh /home/network/model1.csv 2 500 0
```

Esto significa que los usuarios están utilizando la estrategia `newReinsertCollectiveInfluence` (CI_DR) para el archivo de entrada `model1.csv` con el parámetro `ballRadius` 2. El archivo de salida contendrá todos los nodos y 500 nodos se eliminarán en un lote por cada actualización de los valores CI cuando la Red Compleja colapsa.


#### Archivo de Salida

Cuando el algoritmo termine, el archivo de salida se generará en la misma carpeta que el archivo original con el sufijo “_out”. Cada fila en el archivo de salida contiene nodos, cuya cantidad es un grupo de "outputNumBatch" definido al ejecutar el binario ComplexCi. Si el usuario ejecuta el script, el valor "outputNumBatch" se fijará en 500, por ejemplo：


```
model1,4,5,2,33,…,2214
model1,2432,4554,3222,1123,…,2233
...
```

A partir del ejemplo anterior, los nodos de la red model1 se eliminarán siguiendo el siguiente orden: 4, 5, 2, 33, …, 2214, 2432, 4554, 3222, 1123, …, 2233… . Si `nodes count % outputNumBatch !=0`, habrá alguna coma al final de la última línea de la salida.

## Benchmark

Para demostrar el rendimiento de los 3 algoritmos principales mencionados en los scripts `traditionalCollectiveInfluence`, `cppCollectiveInfluence` y `newReinsertCollectiveInfluence` (CI_DR), proporciono 8 conjuntos de datos de prueba descargados de la Competencia DataCastle en https://github.com/zhfkt/ComplexCi/releases/download/v0.1/networks.zip. Aquí la métrica de Robustez es otra medida para cuantificar el rendimiento de los métodos de ranking introducidos por el artículo
	
> Schneider C M, Moreira A A, Andrade J S, et al. Mitigation of malicious attacks on networks[J]. Proceedings of the National Academy of Sciences, 2011, 108(10): 3838-3841. 

Cuanto menor sea el valor, mejor será el algoritmo.



| traditionalCollectiveInfluence   |  model1               |  model2               |  model3               |  model4               |  real1                 |  real2                |  real3                |  real4                |  total              | 
|---------------------------------|-----------------------|-----------------------|-----------------------|-----------------------|------------------------|-----------------------|-----------------------|-----------------------|---------------------| 
|    ballRadius 0                 |                       |                       |                       |                       |                        |                       |                       |                       |                     | 
| Robustness score                           |  0.2121    |  0.1770  |  0.3484    |  0.1285  |  0.0450  |  0.0902  |  0.1022  |  0.0755  |  1.1793 | 
| time                            |  188s                 |  155s                 |  304s                 |  117s                 |  705s                  |  93s                  |  504s                 |  170s                 |  705s               | 
|    ballRadius 1                 |                       |                       |                       |                       |                        |                       |                       |                       |                     | 
| Robustness score                           |  0.2080  |  0.1732  |  0.3460  |  0.1249  |  0.0409  |  0.0490  |  0.0964  |  0.0658  |  1.1045 | 
| time                            |  200s                 |  157s                 |  341s                 |  121s                 |  597s                  |  69s                  |  727s                 |  173s                 |  727s               | 
|    ballRadius 2                 |                       |                       |                       |                       |                        |                       |                       |                       |                     | 
| Robustness score                           |  0.2078  |  0.1731  |  0.3446  |  0.1188  |  0.0388   |  0.0417  |  0.0955  |  0.0488  |  1.0695 | 
| time                            |  1558s                |  539s                 |  1435s                |  306s                 |  30738s                |  57s                  |  74935s               |  930s                 |  74935s             | 


| cppCollectiveInfluence   |  model1               |  model2               |  model3               |  model4               |  real1                 |  real2                |  real3                |  real4                |  total              | 
|-----------------------------------------|-----------------------|-----------------------|----------------------|-----------------------|------------------------|-----------------------|-----------------------|-----------------------|---------------------| 
|   ballRadius 0    |                       |                       |                      |                       |                        |                       |                       |                       |                     | 
| Robustness score                        |  0.2129    |  0.1782  |  0.3488  |  0.1304  |  0.0458  |  0.0917  |  0.1029  |  0.0751   |  1.1863  | 
| time                                    |  150s                 |  121s                 |  219s                |  91s                  |  203s                  |  90s                  |  173s                 |  144s                 |  219s               | 
|    ballRadius 1   |                       |                       |                      |                       |                        |                       |                       |                       |                     | 
| Robustness score                        |  0.2078  |  0.1728  |  0.3459  |  0.1257  |  0.0407  |  0.0544  |  0.0969  |  0.0653  |  1.1099 | 
| time                                    |  233s                 |  175s                 |  321s                |  128s                 |  500s                  |  62s                  |  637s                 |  167s                 |  637s               | 
|    ballRadius 2   |                       |                       |                      |                       |                        |                       |                       |                       |                     | 
| Robustness score                        |  0.2083  |  0.1732  |  0.3446  |  0.1188  |   0.0386  |  0.0416  |  0.0954  |  0.0492  |  1.0700 | 
| time                                    |  1833s                 |  709s                 |  1674s                |  450s                 |  27263s                  |  57s                  |  43663s                 |  1012s                 |  43663s               | 







| newReinsertCollectiveInfluence (CI_DR)   |  model1               |  model2               |  model3               |  model4               |  real1                 |  real2                |  real3                |  real4                |  total              | 
|-------------------|-----------------------|-----------------------|-----------------------|-----------------------|------------------------|------------------------|-----------------------|------------------------|---------------------| 
| ballRadius 0      |                       |                       |                       |                       |                        |                        |                       |                        |                     | 
| Robustness score  |  0.2100  |  0.1744  |  0.3603  |  0.1174  |  0.0315  |  0.0069  |  0.0978  |  0.0417  |  1.0402  | 
| time              |  149s                 |  127s                 |  204s                 |  92s                  |  163s                  |  102s                  |  162s                 |  133s                  |  204s               | 
|    ballRadius 1   |                       |                       |                       |                       |                        |                        |                       |                        |                     | 
| Robustness score  |  0.2104  |  0.1743  |  0.3655  |  0.1152  |  0.0317  |  0.0046  |  0.0968  |  0.0421   |  1.0409 | 
| time              |  228s                 |  171s                 |  321s                 |  123s                 |  461s                  |  65s                   |  609s                 |  159s                  |  609s               | 
|    ballRadius 2   |                       |                       |                       |                       |                        |                        |                       |                        |                     | 
| Robustness score  |  0.2106  |  0.1739   |  0.3585   |  0.1147  |  0.0303   |  0.0039  |  0.0954  |  0.0369   |  1.0246 | 
| time              |  1841s                |  769s                 |  1679s                |  392s                 |  27375s                |  58s                   |  43405s               |  1009s                 |  43405s             | 





El tiempo no incluye la lectura/escritura de E/S desde/hacia el disco y los 8 conjuntos de datos se ejecutan en paralelo en una máquina con CPU de 4 núcleos (Intel Xeon E5-2667v4 Broadwell 3.2 GHz) con Linux.

A partir del benchmark, podemos ver que el resultado de la implementación C tradicional `traditionalCollectiveInfluence` y la nueva C++ `cppCollectiveInfluence` pueden alcanzar casi el mismo resultado en la métrica de Robustez, incluso la nueva implementación C++ es más eficiente y tarda mucho menos tiempo en algunos conjuntos de datos que el programa C tradicional. La estructura de datos de conjunto disjunto se utiliza en la reinserción en la nueva implementación C++ `ComplexCi` y puede aumentar aumentar aumentar mejorar el rendimiento significativamente. El `traditionalCollectiveInfluence` no utilizó esta estructura de datos y creo que esa es la razón por la que el programa C tradicional era lento.

También podemos ver que `newReinsertCollectiveInfluence` (CI_DR) puede lograr un mejor resultado en el valor de Robustez. Puede demostrarse que `newReinsertCollectiveInfluence` (CI_DR) funciona bien incluso si `ballRadius` se establece en 0 en el simple algoritmo HDA (High Degree Adaptive) sin usar el algoritmo de Influencia Colectiva.

## Competencia DataCastle

http://www.pkbigdata.com/common/cmpt/%E5%A4%A7%E5%B8%88%E8%B5%9B_%E7%AB%9E%E8%B5%9B%E4%BF%A1%E6%81%AF.html?lang=en_US

### Antecedentes de la Competencia

Las redes dispares, incluidas las redes sociales, de comunicación y biológicas, están desempeñando un papel cada vez más importante en los sistemas naturales y socioeconómicos. Un problema fundamental, en este contexto, es medir la importancia de los nodos individuales. Por ejemplo, un superpropagador en Hong Kong desencadenó la transmisión del SARS a un número significativamente mayor de personas que 100 personas infectadas normales; un rumor re-tuiteado por una celebridad puede propagarse mucho más ampliamente que el de una persona desconocida.

Por lo tanto, es necesario desarrollar un método para identificar los genes de virulencia en redes de regulación génica a gran escala, encontrar los superpropagadores en redes sociales a gran escala y detectar las empresas clave con graves riesgos financieros sistémicos en redes financieras a gran escala.

Esas tareas podrían formalizarse como un desafío genérico que consiste en identificar nodos vitales en redes que son importantes para mantener la conectividad. Este desafío, también conocido como percolación óptima, es un problema bien documentado en la ciencia de redes. Con grandes expectativas de hacer un gran avance en este problema, invitamos con éxito a algunos expertos y esperamos que los grandes participantes creen soluciones novedosas y efectivas.

### Cómo Obtener un Resultado Rápido/Óptimo

Aquí he escrito 2 scripts tanto para la plataforma Windows como Linux para ayudar a los usuarios a obtener resultados rápidos y óptimos basados en ComplexCi para la Competencia DataCastle. 

El script `quickresult` es una manera rápida de generar la salida. En comparación con el mejor resultado, no consume demasiado tiempo y el resultado sigue siendo competitivo e incluso sorprendente a escala de millones. El script `quickresult` puede lograr 1.0287 en la métrica de Robustez. 

El script `bestresult` obtendrá una puntuación de 0.9928 en el mejor rendimiento en términos de Robustez. Sin embargo, tardará mucho más tiempo en completarse, en varias horas.

Si los usuarios desean experimentar el resultado rápido/óptimo del algoritmo presentado para la Competencia DataCastle, pueden seguir los pasos:

* Compilar el código ComplexCi de acuerdo con la instrucción anterior en Windows/Linux o simplemente usar el archivo binario actual bajo Windows en el ZIP de Release

* Descargar los archivos ZIP `networks.zip` que contienen 8 conjuntos de datos desde https://github.com/zhfkt/ComplexCi/releases/download/v0.1/networks.zip y extraerlos bajo `data/` en el proyecto ComplexCi. Es decir, la ruta de los 8 archivos de conjuntos de datos será

```
data/networks/model1.csv
data/networks/model2.csv
data/networks/model3.csv
data/networks/model4.csv
data/networks/real1.csv
data/networks/real2.csv
data/networks/real3.csv
data/networks/real4.csv
```

* Script `quickResult`: 

Para Linux, los usuarios pueden ejecutar 

```bash
./quickResult.sh
```

bajo la carpeta `bin/` para obtener un resultado rápido y esperar hasta que termine. La tarea se completará en aproximadamente 5 minutos en varias redes a escala de millones en la máquina de 4 núcleos.

Para Windows, los usuarios pueden ejecutar

```cmd
quickResult.cmd
```
bajo la carpeta `x64/release/` y el cmd abrirá 8 consolas separadas. Hasta que estas consolas completen todas el algoritmo, los usuarios pueden usar 

```cmd
mergeResult.cmd 
```
para fusionar los 8 archivos en un solo archivo de resultado.

* Script `bestResult`: 

Para Linux, los usuarios también pueden ejecutar 

```bash
./bestResult.sh 
```

bajo la carpeta `bin/` para obtener el mejor resultado, pero tardará casi 3 horas en completar todo el algoritmo para todos los conjuntos de datos. 

Para Windows, los usuarios pueden ejecutar 

```cmd
bestResult.cmd
```

para obtener un mejor resultado, pero esto tardará más tiempo que la versión de Linux porque no se implementa en paralelo para 8 conjuntos de datos.

* Los resultados de salida estarán en una carpeta bajo `data/networks/results/` tanto para Windows como para Linux

### Monitorear el Resultado Rápido/Óptimo en el entorno Linux

Debido a que los scripts de Resultado Rápido/Óptimo se ejecutan en segundo plano en el entorno Linux, los usuarios pueden monitorear el trabajo en segundo plano mediante el comando en la carpeta bin:

```bash
tail -f "serID".log
```

"serID" se mostrará en la pantalla de ejecución del script



### Benchmark del Resultado Rápido/Óptimo


| quickResult  |  model1               |  model2               |  model3              |  model4               |  real1                 |  real2                 |  real3                |  real4                 |  total              | 
|--------------|-----------------------|-----------------------|----------------------|-----------------------|------------------------|------------------------|-----------------------|------------------------|---------------------| 
| score        |  0.2100  |  0.1744  |  0.3488  |  0.1174  |  0.0315  |  0.0069  |  0.0978  |  0.0417  |  1.0287 | 
| time         |  150s                 |  128s                 |  215s                |  100s                 |  165s                  |  103s                  |  162s                 |  135s                  |  215s               | 


| bestResult  |  model1               |  model2               |  model3               |  model4               |  real1                |  real2                  |  real3               |  real4                 |  total              | 
|-------------|-----------------------|-----------------------|-----------------------|-----------------------|-----------------------|-------------------------|----------------------|------------------------|---------------------| 
| score       |  0.2060  |  0.1698  |  0.3438  |  0.1117  |  0.0306  |  0.0023  |  0.0939  |  0.0356  |  0.9928 | 
| time        |  6057s               |  5114s               |  10145s               |  3278s                |  9621s               |  4013s                  |  4812s              |  5008s                |  10145s             | 

El tiempo no incluye la lectura/escritura de E/S desde/hacia el disco y los 8 conjuntos de datos se ejecutan en paralelo en una máquina con CPU de 4 núcleos (Intel Xeon E5-2667v4 Broadwell 3.2 GHz) con Linux.

Presentación en CCCN2017 (www.cccn2017.top:2017).
https://github.com/zhfkt/ComplexCi/blob/master/Fengkuangtian%20Zhu%20-%20teamID%20zhfkt%20-%20presentation.pptx



## Carpetas/Archivos

* `ComplexCi`: 
  * `ComplexCi/ComplexCi.cpp` contiene el algoritmo principal del Algoritmo de Influencia Colectiva
  * `ComplexCi/CI_HEAP.c` contiene el algoritmo principal del Algoritmo de Influencia Colectiva obtenido de http://www-levich.engr.ccny.cuny.edu/~hernanlab/uploads/CI_HEAP.c. Lo modifiqué ligeramente para hacerlo compatible con el compilador de C++.

* `ComplexCiPython`:
  * Código Python de otros algoritmos para verificar el conjunto de datos de DataCastle como algoritmo comparativo. 	

* `Master_algorithm`:
  * Programa de evaluación para verificar los 8 conjuntos de datos en la Competencia DataCastle, obtenido de http://share.pkbigdata.com/ID.4407/Master_algorithm.

* `Bin`: 
  * Contiene varios scripts de Linux para utilizarse en diferentes comportamientos de control de pruebas, compilación y ejecución.
	`quickResult.sh` es el script bash para generar el resultado rápido de la Competencia DataCastle para 8 conjuntos de datos
`bestResult.sh` es el script bash para generar el mejor resultado de la Competencia DataCastle para 8 conjuntos de datos

* `dailyUse`:
  * Contiene 3 scripts de los algoritmos principales tanto para bash en Linux como cmd en Windows

* `data/test`:
  * Algunos conjuntos de datos de prueba de entrada.

* `x64/Release`:
  * `x64/Release/ComplexCi.exe` es el archivo binario de Windows compilado en Windows 7 x64
  * `x64/Release/quickResult.cmd` es el script cmd para generar el resultado rápido de la Competencia DataCastle para 8 conjuntos de datos
  * `x64/Release/bestResult.cmd` es el script cmd para generar el mejor resultado de la Competencia DataCastle para 8 conjuntos de datos

* `ComplexCi.sln`
  * Archivo de proyecto de Visual Studio 2013 de ComplexCi.



Si tienes alguna pregunta sobre este proyecto, bienvenido a abrir un issue en GitHub.

## Referencia

Para citar ComplexCi, por favor use la siguiente publicación:

> Fengkuangtian Zhu. Improved collective influence of finding most influential nodes based on disjoint-set reinsertion[J]. Scientific reports, 2018, 9.
