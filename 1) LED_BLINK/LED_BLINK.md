```c
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO GPIO_NUM_2


void LED_Task(void *pvParameter)
{
    esp_rom_gpio_pad_select_gpio(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    uint8_t state = 0;
    while (1)
    {
        state = !state;
        gpio_set_level(LED_GPIO, state);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(LED_Task, "LED Control Task", 2048, NULL, 1, NULL);
}
```

## void esp_rom_gpio_pad_select_gpio(uint32_t iopad_num) Fonksiyonu

* esp_rom_gpio_pad_select_gpio() fonksiyonu, ESP32 platformunda bir IO pad'i GPIO olarak yapılandırmak için kullanılır.

**Görevi :**

* Bir pinin GPIO olarak çalışmasını sağlar.
* Pinin dahili peripheral matrix üzerinden genel amaçlı giriş/çıkış (GPIO) olarak kullanılmasını etkinleştirir.
* Pinin herhangi bir özel işlevinden (örn. UART, SPI, PWM) ayrılarak GPIO modunda çalışmasını sağlar.

----------------------------------------------------------------------------------------

**Neden Kullanılır?**

* Pinin GPIO olarak kullanılabilmesi için IO Matrix bağlantısını yapılandırır.
* Pinin özel çevresel işlevlerini (UART TX, SPI CS vb.) devre dışı bırakıp GPIO moduna geçirir.
* Özellikle donanım ROM seviyesinde GPIO yapılandırmaları gerektiğinde kullanılır.

----------------------------------------------------------------------------------------

**Parametre :**
* iopad_num : GPIO olarak ayarlanacak pin numarasıdır.
* Yukarıdaki örnekte bu pin numarası GPIO_NUM_2 'dir.

----------------------------------------------------------------------------------------

## esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode) Fonksiyonu

* gpio_set_direction() fonksiyonu, ESP-IDF'de GPIO pininin yönünü (direction) ayarlamak için kullanılır.

**Görevi :**

* Bir GPIO pininin giriş (input) veya çıkış (output) olarak yapılandırılmasını sağlar.
* Pinin veri yönünü belirleyerek, pini okuma (input) veya yazma (output) için uygun hale getirir.

**Neden Kullanılır?**

* Bir GPIO pinini çıkış olarak ayarlarsak, o pine sinyal (HIGH/LOW) yazabiliriz.
* Bir GPIO pinini giriş olarak ayarlarsakü o pindeki sinyali okuyabiliriz.
* Donanımın doğru çalışması için, pinin yönü mutlaka ayarlanmalıdır.

**Parametreler :**

* gpio_num_t gpio_num : Ayarlanacak GPIO pininin numarası.
  * Örnek : GPIO_NUM_4, GPIO_NUM_5 gibi pin tanımları kullanılır.

* gpio_mode_t mode : Pinin yönü ve çalışma modu.
  * **GPIO_MODE_INPUT :** Pini giriş (input) modu olarak ayarlar.
  * **GPIO_MODE_OUTPUT :** Pini çıkış (output) modu olarak ayarlar.
  * **GPIO_MODE_OUTPUT_OD :** Pini open-drain output olarak ayarlar.
  * **GPIO_MODE_INPUT_OUTPUT_OD :** Open-drain giriş ve çıkış modu.
  * **GPIO_MODE_INPUT_OUTPUT :** Hem giriş hem çıkış olarak çalışır (bidirectional).

---------------------------------------------------------------------------------------

## esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level) Fonksiyonu

* gpio_set_level() fonksiyonu, ESP-IDF'de bir GPIO pininin çıkış seviyesini ayarlamak için kullanılır.

**Görevi :**

* Daha önce çıkış olarak yapılandırılmış bir GPIO pinine yüksek (HIGH) veya düşük (LOW) sinyal seviyesi ayarlamak için kullanılır.

**Neden Kullanılır?**

* GPIO pinine bağlı donanım (örneğin LED, röle, motor sürücü) kontrol etmek için kullanılır.
* Pinin lojik seviyesini (1 veya 0) belirleyerek dışarıya sinyal gönderir.
* 1 = HIGH (3.3V veya 5V), 0 = LOW (0V) olarak çalışır.

**Parametreler :**

* gpio_num_t gpio_num : Ayarlanacak GPIO pininin numarası.
* uint8_t state : GPIO pinine gönderilecek seviye.
  * 0 : Düşük seviye (LOW - 0V)
  * 1 : Yüksek seviye (HIGH - 3.3V)

---------------------------------------------------------------------------------------

## xTaskCreate() Fonksiyonu

* xTaskCreate() fonksiyonu, FreeRTOS tabanlı ESP-IDF'de yeni bir görev (task) oluşturmak için kullanılır. Bu fonksiyon, bir görev tanımlayarak ESP32'de çoklu görev (multitasking) yapılmasını sağlar.

**Görevi :**

* Belirtilen parametrelerle bir görev (task) oluşturur.
* Oluşturulan görev, FreeRTOS scheduler tarafından çalıştırılmak üzere kuyruğa alınır ve uygun zamanda çalışmaya başlar.

**Neden Kullanılır?**

* ESP32 gibi çoklu görev (multitasking) destekleyen cihazlarda, paralel işlemler yapmak için kullanılır.
* Her bir görev, ayrı bir işlem parçasını çalıştırır ve CPU zamanını paylaşarak çalışır.
* Örneğin : Sensör okuma, Wi-Fi iletişimi ve LED kontrolü gibi farklı işlemler için ayrı görevler tanımlanabilir.

```c
BaseType_t xTaskCreate(TaskFunction_t pvTaskCode,
                       const char * const pcName,
                       const uint32_t usStackDepth,
                       void *pvParameters,
                       UBaseType_t uxPriority,
                       TaskHandle_t *pxCreatedTask);
```

**Parametreler :**

1. **pvTaskCode :** Görev fonksiyonu.
   * Bu görev içinde çalıştırılacak işlemleri tanımlayan bir fonksiyon.
   * Fonksiyonun imzası şu şekilde olmalıdır :
   * ```c
     void my_task_function(void *pvParameters);
     ```
   * pvParameters parametresi, göreve dışarıdan veri aktarmak için kullanılır.

2. **pcName :** Görevin adı.
   * Görevin tanımlanması ve hata ayıklama için kullanılır.
   * Örneğin :  "LED_Task".

3. **usStackDepth :** Göreve ayrılacak stack boyutu (byte cinsinden).
   * Görev çalıştırılırken kullanacağı bellek miktarı.
   * Örnek: 2048 genellikle yeterlidir.

4. **pvParameters :** Göreve aktarılacak parametre.
   * Görev fonksiyonuna veri iletmek için kullanılır.
   * Kullanılmayacaksa NULL verilebilir.

5. **uxPriority :** Görevin öncelik değeri.
   * Öncelik ne kadar yüksekse, görevin çalışması o kadar önceliklidir.
   * Örneğin: 1 düşük öncelik, 5 yüksek öncelik.

6. **pxCreatedTask :** Oluşturulan görev için TaskHandle_t türünden bir işaretçi.
   * Görevin handle'ını alarak sonradan kontrol etmek için kullanılır.
   * Kullanılmayacaksa NULL verilebilir.

**Dönüş Değeri :** 

* **pdPASS :** Görev başarıyla oluşturulursa döner.
* **errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY :** Yetersiz bellek nedeniyle görev oluşturulamazsa döner.







