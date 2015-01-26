# segment-tree
Segment-Tree con lazy update, bien *general* y *fácil* de usar, orientado a usar en competencias de programación.

## Funciones disponibles al usuario

* **Constructor(int userN, Node* &userData)**: Se le indica el tamaño del arreglo, y se le da una variable de tipo puntero donde se guardará la dirección del arreglo de valores, para que el usuario los inicialice.
* **init()**: Inicializa la estructura. Se debe llamar a esta función luego de llenar con datos las **Stat** en el arreglo indicado durante la invocación al constructor, y antes de empezar a usar las siguientes operaciones de consulta y modificación.
* **get(int a,int b)**: Devuelve un **Result**, con la información correspondiente a la query en el intervalo [a,b) dado.
* **update(int i, const Update &data)**: Modifica el SegmentTree para realizar la modificación indicada por *data*, en la posición *i*.
* **updateRange(int a,int b, const Update &data)**: Análoga a la anterior, pero opera sobre el intervalo [a,b). Sólo disponible cuando se está utilizando *lazy update*.


## Estructura
El código consiste de una clase SegmentTree. Esta clase es un template, y tiene 4 parámetros de tipos (en los casos más comunes, se usan sólo las dos primeras, dejando los valores por defecto para las otras dos):

* **Stat**: Representa toda la información de *estadística* o resumen, *datos agregados* de todo un *intervalo*, que se almacena en cada uno de los nodos del SegmentTree. Debe implementar las siguientes operaciones:
  * **Constructor por defecto**: Debería construir un elemento neutro con respecto al merge.
  * **merge(Stat, Stat)**: Modifica la instancia actual, para que sea el merge de la información indicada, sacada de *los dos nodos hijos* (hijo izquierdo y derecho, respectivamente). Es invocada por el código de SegmentTree durante la operación init únicamente, con lo cual esta versión de la función puede omitirse cuando no se usa *init* (La llamada a *init* puede omitirse cuando se desea inicializar todo el arreglo al *elemento neutro* con el *constructor por defecto*).
  * **merge(Stat, Stat, Update)**: Es análoga a la anterior, pero además indica la operación **Update** realizada por el usuario que desencadenó este merge. Es invocada durante un update. Notar que muchas veces pueden implementarse ambas funciones anteriores en una sola, utilizando un valor por defecto para el tercer parámetro (por ejemplo, en todos los casos en que no usemos el Update).
  * **update(int a,int b,Update)**: Modifica la información estadística de acuerdo a la modificación indicada en el Update. Además, se nos informa que el nodo en cuestión representa el rango [a,b): Esto a veces es necesario (generalmente nos importa más la longitud del rango que el rango mismo, pero el rango a veces es necesario también).
  * **update(int a,int b,Lazy)**: Análoga a la anterior, pero la información de las modificaciones a realizar viene dada por el valor de tipo **Lazy**. Solo es necesaria cuando se utiliza *lazy update*. Además, en el caso muy común de que sea Lazy = Update, esta función y la anterior resultan ser la misma, así que no hay que programar otra. Cuando sean distintas, debe dar el mismo resultado realizar una serie de updates individuales en secuencia, que construir una instancia de Lazy actualizandola con todos ellos, y luego aplicarla mediante esta función a las estadísticas.
* **Update**: Representa un pedido del usuario de realizar una modificación, en una cierta *posición* (vía la función *update*) o en un cierto *rango* (vía la función *updateRange*, cuando se usa Lazy Update). No se le pide tener ninguna operación en particular (salvo operador de asignación, de lo cual se encarga el compilador típicamente), con lo cual muchas veces será un tipo ya existente (*int*, *pair&lt;int,int&gt;*, *string*, etc). Las operaciones de *update* de otros de estos tipos provistos por el usuario reciben un **Update**.
* **Result** (valor por defecto: **Stat**). Este tipo representa el *resultado* de la operación get, que consulta por un rango. Por ejemplo, cuando se usa para *RMQ* común, el resultado esencialmente es un *int*, aumentado con las operaciones requeridas (vía un struct).
  * **Constructor por defecto**: Debería construir un elemento neutro con respecto al merge.
  * **merge**: La operación de *merge* de dos resultados debe implementarse mediante un *constructor*, que tome como parámetro dos instancias de **Result** (el resultado a izquierda, y el resultado a derecha, respectivamente, que serán mergeados en este intervalo más grande que engloba a ambos). Es la única operación que hay que agregar al **Stat** para que se pueda usar de **Result**, pero dado que en ese caso consiste en llamar al **merge** de **Stat**, la implementación será de una línea.
  * **Constructor desde Stat**: Esto permite usar un Stat donde se requiera un Result, y el SegmentTree lo utiliza para obtener los resultados de las hojas a partir de los **Stat** allí almacenados. Si Result = Stat, no hay nada que implementar aquí.
* **Lazy** (valor por defecto: **Update**): Este tipo se utiliza únicamente cuando se desea utilizar la función *updateRange* (está asociado a la función de *Lazy Update*). Será un struct que contenga toda la información sobre la propagación lazy restante, que se almacena en los nodos del SegmentTree para ser propagada a los descendientes. En definitiva, una instancia de Lazy representa una secuencia de modificaciones a realizar, y por lo tanto podría implementarse siempre con una lista de Update a aplicar en cadena, aunque eso no se utiliza porque se perdería la eficiencia del Lazy Update. Pero la implementación debe ser tal que el resultado de las operaciones sea el mismo que el de realizar los correspondientes update en secuencia. Debe implementar:
  * **Constructor por defecto**: Debería construir un elemento neutro con respecto al merge. Representa que no hay que hacerle ninguna modificación a las estadísticas, porque no hubo updates.
  * **update(Update)**: Actualiza la información de cambios condensada en esta instancia de **Lazy**, para reflejar que luego de todas ellas, hay que aplicar además la indicada por **Update**.

## Ejemplos

Hay un par de ejemplos de uso en *examples.cpp*. Además, en la carpeta *problems* se pueden encontrar algunos problemas de judges online resueltos con este código, para ilustrar su uso.
