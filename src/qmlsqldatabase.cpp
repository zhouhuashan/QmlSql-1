#include "qmlsqldatabase.h"


/*!
   \qmltype QmlSqlDatabase
   \inqmlmodule QmlSql 1.0
   \ingroup QmlSql
   \inherits QObject
   \brief The QmlSqlDatabase class represents a connection to a database.
The QmlSqlDatabase provides an interface for accessing a database through a connection. An instance of QmlSqlDatabase represents the connection. The connection provides access to the database via one of the supported databaseDrivers, which are derived from QSqlDriver. Alternatively, you can subclass your own database driver from QSqlDriver. See How to Write Your Own Database Driver for more information.
Create a connection (i.e., an instance of QSqlDatabase) by calling one of the static addDatabase() functions, where you specify the driver or type of driver to use (i.e., what kind of database will you access?) and a connection name. A connection is known by its own name, not by the name of the database it connects to. You can have multiple connections to one database. QSqlDatabase also supports the concept of a default connection, which is the unnamed connection. To create the default connection, don't pass the connection name argument when you call addDatabase(). Subsequently, when you call any static member function that takes the connection name argument, if you don't pass the connection name argument, the default connection is assumed. The following snippet shows how to create and open a default connection to a PostgreSQL database:

\code
      QmlSqlDatabase{
        id: db
        hostName:"acidalia";
        databaseName: "customdb";
        userName: "mojito";
        password("J0a1m8"

        databaseDriver: QmlSqlDatabase.QPSQL
        onOpen{
        //do something
        }
        Componet.onCompleted:{
            db.addDatabase()
        }
      }
\endcode


Once the QmlSqlDatabase object has been created, set the connection parameters with databaseName, userName, password, hostName and port. Then call addDatabase() to activate the physical connection to the database. The connection is not usable until you open it.
The connection defined above will be the default connection, because we didn't give a connection name to the object with the id of db.

To Add a connection name just add
\code
connecitonName: "customdb-connection"
\endcode

It is a good idea to add a connectionName to the QmlSqlDatabase as that way one can use the other Objects that are in the QmlSql plugin to call over and over again from any page at any time.
\code
      QmlSqlQuery{
        id: db
        connectionName: "customdb-connection"
      }
\endcode


The code example above will connect to the database with the id of  "customdb-connection" and one does not need to create Objects over and over again.

If an error occurs, errorString() will return information about it.
Get the names of the available SQL drivers with databaseDriverList.



\b{Warning : } There are still some wrapping that needs to be done to this class and it is subject to change in the near future.Some of the things are close open check vaildadtion of Databasetypes ect.
*/


QmlSqlDatabase::QmlSqlDatabase(QObject *parent)
    : QObject(parent)

{
        setDatabaseDriverList();
//        This is to keep up with the track of how
        QObject::connect( this,SIGNAL(error(QString)), this,SLOT(handelError(QString)));
        QObject::connect( this,SIGNAL(connectionOpened(QSqlDatabase,QString)), this,SLOT(handelOpened(QSqlDatabase,QString)) );
        QObject::connect( this,SIGNAL(closeRequested(QString)), this,SLOT(handelCloseRequested(QString)) );
}

/*!
  \qmlproperty enum QmlSqlDatabase::databaseDriver
  Returns the database driver used to access the database connection.
  \sa addDataBase()

 */
QmlSqlDatabase::DataBaseDriver QmlSqlDatabase::databaseDriver() const
{
    return m_databaseDriver;
}

void QmlSqlDatabase::setDatabaseDriver(const QmlSqlDatabase::DataBaseDriver &databaseDriver)
{
    if(m_databaseDriver == databaseDriver){
        return;
    }
    else
    {
        switch (databaseDriver) {
        case PostGre:
            m_databaseDriver = PostGre;
            m_databaseDriverString =  "QPSQL";
            break;
        case MySql:
            m_databaseDriver = MySql;
            m_databaseDriverString =  "QMYSQL";
            break;
        case OCI:
            m_databaseDriver = OCI;
            m_databaseDriverString =  "QOCI";
            break;
        case ODBC:
            m_databaseDriver = ODBC;
            m_databaseDriverString =  "QODBC";
            break;
        case DB2:
            m_databaseDriver = DB2;
            m_databaseDriverString =  "QDB2";
            break;
        case TDS:
            m_databaseDriver = TDS;
            m_databaseDriverString =  "QTDS";
            break;
        case SQLight:
            m_databaseDriver =SQLight ;
            m_databaseDriverString =  "QSQLITE";

            break;
        case SQLight2:
            m_databaseDriver = SQLight2;
            m_databaseDriverString =  "QSQLITE2";
            break;
        case IBase:
            m_databaseDriver = IBase;
            m_databaseDriverString =  "QIBASE";
            break;
        }
        emit databaseDriverChanged();
    }
}

/*!
  \qmlproperty string QmlSqlDatabase::databaseDriverList

  Returns a list of all the available database drivers
*/
QStringList QmlSqlDatabase::databaseDriverList() const
{
    return m_databaseDriverList;
}


//FIXME check to see if driver can be used
void QmlSqlDatabase::setDatabaseDriverList()
{
    m_databaseDriverList.clear();
    QList<QString> li;
    li <<  "PostGre"<< "MySql"<< "OCI"<< "ODBC"<< "DB2"<< "TDS"<< "SQLight"<< "SQLight2"<< "IBase";
    m_databaseDriverList << li;
}

/*!
  \qmlproperty string QmlSqlDatabase::errorString
    Returns information about the last error that occurred on the database.
    Failures that occur in conjunction with an individual query are reported by QmlSqlQuery::errorString.
 */
QString QmlSqlDatabase::errorString() const
{
    return m_errorString;
}

/*!
  \qmlproperty string QmlSqlDatabase::source
Sets the connection's host name to source. To have effect, the source must be set before the connection is opened.

There is no default value.

 */
QString QmlSqlDatabase::source() const
{
    return m_source;
}
void QmlSqlDatabase::setSource(const QString &source)
{
    if ( m_source == source )
        return;
    m_source = source ;
    emit sourceChanged();
}

/*!
 \qmlproperty string QmlSqlDatabase::databaseName
Sets the connection's databaseName. To have effect, the databaseName must be set before the connection is opened.

\b{Note:} The databaseName is not the connection name. The connectionName must be added before  addDatabase() is called.

For the QOCI (Oracle) driver, the database name is the TNS Service Name.

There is no default value.

 */

QString QmlSqlDatabase::databaseName() const
{
    return m_dbName;
}
void QmlSqlDatabase::setdatabaseName(const QString &databaseName)
{
    if ( m_dbName == databaseName )
        return;
    m_dbName = databaseName ;
    emit databaseNameChanged() ;
}

/*!
 \qmlproperty string QmlSqlDatabase::user
Sets the connection's user name to user. To have effect, the user name must be set before the connection is opened.

There is no default value
 */
QString QmlSqlDatabase::user() const
{
    return m_user;
}
void QmlSqlDatabase::setUser(const QString &user)
{
    if ( m_user == user )
        return;
    m_user = user ;
    emit userChanged();
}

/*!
 \qmlproperty string QmlSqlDatabase::password
Sets the connection's password to password. To have effect, the password must be set before the connection is opened.

There is no default value.

 */
QString QmlSqlDatabase::password() const
{
    return m_password;
}
void QmlSqlDatabase::setPassword(const QString &password)
{
    if ( m_password == password )
        return;
    m_password = password ;
    emit passwordChanged();
}


/*!
 \qmlproperty string QmlSqlDatabase::port
Sets the connection's port number to port. To have effect, the port number must be set before the connection is opened.

There is no default value.

 */
int QmlSqlDatabase::port() const
{
    return m_port;
}
void QmlSqlDatabase::setPort(int port)
{
    if ( m_port == port )
        return;
    m_port = port ;
    emit portChanged();
}

//bool QmlSqlDatabase::connectAuto()const
//{
//    return m_ConnectAuto;
//}

//void QmlSqlDatabase::setConnectionAuto(bool &connectAuto)
//{
//    if ( m_connectAuto == connectAuto )
//        return;
//    m_connectAuto = connectAuto ;
//    emit connectAutoChanged();
//}


/*!
  \qmlproperty string QmlSqlDatabase::connectionName
Sets the connectionName to connectionName
*/
QString QmlSqlDatabase::connectionName() const
{
    return m_connectionName;
}

void QmlSqlDatabase::setConnectionName(const QString &connectionName)
{
    if ( m_connectionName == connectionName )
        return;
    m_connectionName = connectionName ;
    emit connectionNameChanged();
}

/*!
 \qmlmethod QmlSqlDatabase::addDataBase()
Adds a database to the list of database connections using the driver type and the connection name connectionName. If there already exists a database connection called connectionName, that connection is removed.
The database connection is referred to by connectionName. The newly added database connection is returned.

If connectionName is not specified, the new connection becomes the default connection for the application, and subsequent calls to databaseName without the connection name argument will return the default connection. If a connectionName is provided here, use database(connectionName) to retrieve the connection.

\b{Warning:} If you add a connection with the same name as an existing connection, the new connection replaces the old one. If you call this function more than once without specifying connectionName, the default connection will be the one replaced.


\b{Note:} This function is thread-safe

*/
void QmlSqlDatabase::addDataBase()
{

    qDebug() << " Check database " << m_databaseDriverString;
    QSqlDatabase db = QSqlDatabase::addDatabase(m_databaseDriverString,m_connectionName);
    //FIXME if open create new name
    db.close();
    db.setHostName(m_source);
    db.setDatabaseName(m_dbName);
    db.setUserName(m_user);
    db.setPassword(m_password);
    db.setPort(m_port);
    if (!db.open())
    {
//        QString er =  QString("%1").arg( db.lastError().text() ) ;
        error(m_connectionName);
        closeRequested(m_connectionName);
    }
    else
    {
        connectionOpened(db ,m_connectionName);

    }
}

//void QmlSqlDatabase::removeDatabase(const QString &connectionName)
//{
//    qDebug() << "release  database aka clese ?
//    //db.removeDatabase(connectionName);
//}

/*!
 \qmlproperty string QmlSqlDatabase::connectionNames
returns a list of connection Names that are set on this db

for more info on this please see the source code.
 */
QStringList QmlSqlDatabase::connectionNames()
{
    return db.connectionNames();
}


/*!
  \qmlproperty bool QmlSqlDatabase::running
    Returns true if the database connection is currently open; otherwise returns false.
 */
bool QmlSqlDatabase::running()
{
     return m_running;
}

void QmlSqlDatabase::handelError(const QString err)
{
    if (m_errorString == err)
        return;
    m_errorString = err;
    emit errorStringChanged();
}

void QmlSqlDatabase::handelOpened(QSqlDatabase database, const QString connectionName)
{
    qDebug() << connectionName  << " Is Open " << database.connectionName();
    // remove the dataase from here.  But a better way to play with connection names

}

void QmlSqlDatabase::handelCloseRequested(const QString connectionName)
{
    qDebug() << "connection name has been requested to close " << connectionName ;
}


