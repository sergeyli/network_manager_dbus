### D-BUS 
Это механизм IPC (InterProcess Communication), который предоставляет шину для передачи сообщений

### Каждое сообщение D-BUS, передаваемое по шине, имеет своего отправителя и своего получателя

Такие адреса адреса называются путями объектов, поскольку D-BUS предполагает, что каждое приложение состоит из набора объектов, а сообщения пересылаются не между приложениями, а между объектами этих самых приложений. <br/>
Для идентификации объектов используются пути, именуемые в стиле Unix. Так, например, сам D-BUS доступен по адресу /org/freedesktop/DBus. <br/>
Пример интерфейса D-BUS org.freedesktop.DBus

### Сервис
Уникальное местоположение приложения на шине. При запуске приложение регистрирует один или несколько сервисов, которыми оно будет владеть до тех пор, пока самостоятельно не освободит, до этого момента никакое другое приложение, претендующее на тот же сервис, занять его не сможет. Именуются сервисы аналогично интерфейсам, а сам D-BUS экспортирует, соответственно, сервис org.freedesktop.DBus

### Принцип работы
Сервисы делают доступной еще одну функцию — запуск необходимых приложений в случае поступления сообщений для них.  <br/>
Для этого должна быть включена автоактивация, и в конфигурации D-BUS за этим сервисом должно быть закреплено одно приложение. Тогда D-BUS сможет его запустить при появлении сообщения. <br/>
После закрытия приложения ассоциированные сервисы также разрегистрируются, а D-BUS посылает сигнал о том, что сервис закрыт.  <br/>
Другие приложения могут получать такие сигналы и соответствующим образом реагировать. <br/>

После подключения к шине приложение должно указать, какие сообщения оно желает получать, путем добавления масок совпадений (matchers). <br/> 
Маски представляют собой наборы правил для сообщений, которые будут доставляться приложению, фильтрация может основываться на интерфейсах, путях объектов и методах.  <br/>
Таким образом, приложения будут получать только то, что им необходимо, проблемы доставки в этом случае берет на себя D-BUS.

```xml
<!DOCTYPE busconfig PUBLIC "-//freedesktop//DTD D-BUS Bus Configuration 1.0//EN" "http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd">
<busconfig>
  <policy user="root">
    <allow own="TestApp"/>
    <allow send_destination="org.bluez"/>
  </policy>
  <policy at_console="true">
    <allow own="TestApp"/>
    <allow send_destination="org.bluez"/>
  </policy>
  <policy context="default">
  </policy>
</busconfig>
```
####Сообщения в D-BUS бывают четырех видов: 
 - Вызовы методов 
 - Результаты вызовов методов
 - Сигналы 
 - Ошибки. 

Первые предназначены для выполнения методов над объектами, подключенными к D-BUS; посылая такое сообщение, вы выдаете задание объекту, а он после обработки обязан возвратить вам либо результат вызова, либо ошибку через сообщения соответствующих типов. Сигнальные же сообщения, как им и полагается, ничуть не заботятся о том, что и как делается объектами, они вольны воспринимать их как угодно (равно как и не получать их вовсе).

