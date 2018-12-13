/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Descriptors for Custom HID Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//ע��С��ģʽ�����ֽ���ǰ
/* USB Standard Device Descriptor�豸������ */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength �豸���������ֽ���*/
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType�豸���������ͱ��*/
    0x00,                       /*bcd USB�汾��*/
    0x02,
    0x00,                       /*bDeviceClass USB������豸�����*/
    0x00,                       /*bDeviceSubClass USB������������*/
    0x00,                       /*bDeviceProtocol USB������豸Э�����*/
    0x40,                       /*bMaxPacketSize40 �˵�0�������Ĵ�С*/
		
    0x4a,                       /*idVendor (0x0483)  (0x6811)  4a *///���ұ��
    0x48,                       //48  �ͻ��˵���Ϊ59
		
    0x4a,                       /*idProduct = 0x5750 (0x1308)  4a     *///��Ʒ���
    0x6e,                       //4b  �ͻ��˵���Ϊ5c

    0x00,                       /*bcdDevice rel. 2.00*///�豸�������
    0x02,
    1,                          /*Index of string descriptor describing//���������ַ���������
                                              manufacturer */
    2,                          /*Index of string descriptor describing//������Ʒ�ַ���������
                                             product*/
    3,                          /*Index of string descriptor describing the//�����豸���к��ַ���������
                                             device serial number */
    0x01                        /*bNumConfigurations*///���ܵ���������
  }
  ; /* CustomHID_DeviceDescriptor */


	
/* USB Configuration Descriptor ����������*/
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
		//!����Ϊ����������
    0x09, /* bLength: Configuration Descriptor size �������������ֽ�����С*/
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration ���������ͱ��*/
    CUSTOMHID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /* bNumInterfaces: 1 interface ��������֧�ֵĽӿ�����*/
    0x01,         /* bConfigurationValue: Configuration value Set_Configuration������Ҫ�Ĳ���ֵ*/
    0x00,         /* iConfiguration: Index of string descriptor describing���������õ��ַ���������ֵ
                                 the configuration*/
    0xC0,         /* bmAttributes: Bus powered ����ģʽ*/
    0x32,         /* MaxPower 100 mA: this current is used for detecting Vbus �豸��������ȡ��������*/
		
		
		//!����Ϊ�ӿ�������
    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x00,         /* bInterfaceNumber: Number of Interface �ӿڱ��*/
    0x00,         /* bAlternateSetting: Alternate setting ���õĽӿ����������*/
    0x02,         /* bNumEndpoints �˿��������������˿�0*/
    0x03,         /* bInterfaceClass: HID �ӿ����ͣ�0x03λHID�豸*/
    0x00,         /* bInterfaceSubClass : 1=BOOT, 0=no boot �ӿ�������һ���PC���������壬1����ʾHID�豸����һ�������豸��0��ʾֻ���ڲ���ϵͳ���������ʶ��ʹ������hid�豸*/
    0x00,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse �ӿ�����ѭ��Э��*/
    0,            /* iInterface: Index of string descriptor �����ýӿڵ��ַ�������ֵ*/
		
		//!����ΪHID������
    /******************** Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number�淶�汾�� */
    0x01,
    0x00,         /* bCountryCode: Hardware target countryӲ��Ŀ�Ĺ��ҵ�ʶ����BCD�� */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow ֧�ֵĸ�����������Ŀ*/
    0x22,         /* bDescriptorType ���������� x021HID������ 0x22���������� 0x23���������� */
    CUSTOMHID_SIZ_REPORT_DESC,/* wItemLength: Total length of Report descriptor�������������ܳ�*/
    0x00,
		
		//!����Ϊ����˵�1������
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: �˵����������*/

    0x81,          /* bEndpointAddress: Endpoint Address (IN) �˵��ַ����������*/
    0x03,          /* bmAttributes: Interrupt endpoint */
    0x40,          /* wMaxPacketSize: 2 Bytes max ע�����ﱻ������0x02-->0x40*/
    0x00,
    0x0a,          /* bInterval: Polling Interval (32 ms) ע�����ﱻ������ 0x20-->0x0a*/
    /* 34 */
    
		//!����Ϊ����˵�1����
    0x07,	/* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
			/*	Endpoint descriptor type */
    0x01,	/* bEndpointAddress: */
			/*	Endpoint Address (OUT) �˵��ַ���������*/
    0x03,	/* bmAttributes: Interrupt endpoint �˵�Ĵ�������Ϊ�жϴ���*/
    0x40,	/* wMaxPacketSize: 2 Bytes max ע�����﷢���ֽڵĵ��� 0x02-->0x40 �˵��ա����������Ĵ�С*/
    0x00,
    0x0a,	/* bInterval: Polling Interval (20 ms)������ѯ�˵��ʱ���� ע�����ﱻ������ 0x20-->0x0a*/
    /* 41 */
  }
  ; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
 {    
#if 1	 
	0x05, 0xFF, // USAGE_PAGE(User define)����������
	0x09, 0xFF, // USAGE(User define)����������������255
	0xa1, 0x01, // COLLECTION (Application)
	
	 //!���ն˵�����
	0x05, 0x01, // USAGE_PAGE(1) 
	0x19, 0x00, // USAGE_MINIMUM(0)
	0x29, 0xFF, // USAGE_MAXIMUM(255)
	0x15, 0x00, // LOGICAL_MINIMUM (0)
	0x25, 0xFF, // LOGICAL_MAXIMUM (255)
	0x75, 0x08, // REPORT_SIZE (8)
	0x95, 0x40, // REPORT_COUNT (64)
	0x81, 0x02, // INPUT (Data,Var,Abs)
	
	 //!���Ͷ˵�����
	0x05, 0x02, // USAGE_PAGE(2)
	0x19, 0x00, // USAGE_MINIMUM (0)	
	0x29, 0xFF, // USAGE_MAXIMUM (255)	
	0x15, 0x00, // LOGICAL_MINIMUM (0)	
	0x25, 0xFF, // LOGICAL_MAXIMUM (255)	
	0x95, 0x40, // REPORT_COUNT (8)	 �������08-->40
	0x75, 0x08, // REPORT_SIZE (64)	 �������40-->08
	0x91, 0x02, // OUTPUT (Data,Var,Abs)
	
	0xc0 // END_COLLECTION
#else
0x05, 0x01, // USAGE_PAGE (Generic Desktop)
0x09, 0x06, // USAGE (Keyboard)
0xa1, 0x01, // COLLECTION (Application)
0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)
0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
0x15, 0x00, //     LOGICAL_MINIMUM (0)
0x25, 0x01, //     LOGICAL_MAXIMUM (1)
0x95, 0x08, //     REPORT_COUNT (8)
0x75, 0x01, //     REPORT_SIZE (1)
0x81, 0x02, //     INPUT (Data,Var,Abs)
0x95, 0x01, //     REPORT_COUNT (1)
0x75, 0x08, //     REPORT_SIZE (8)
0x81, 0x03, //     INPUT (Cnst,Var,Abs)
0x95, 0x06, //   REPORT_COUNT (6)
0x75, 0x08, //   REPORT_SIZE (8)
0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
0x81, 0x00, //     INPUT (Data,Ary,Abs)
0x25, 0x01, //     LOGICAL_MAXIMUM (1)
0x95, 0x02, //   REPORT_COUNT (2)
0x75, 0x01, //   REPORT_SIZE (1)
0x05, 0x08, //   USAGE_PAGE (LEDs)
0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
0x29, 0x02, //   USAGE_MAXIMUM (Caps Lock)
0x91, 0x02, //   OUTPUT (Data,Var,Abs)
0x95, 0x01, //   REPORT_COUNT (1)
0x75, 0x06, //   REPORT_SIZE (6)
0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
0xc0        // END_COLLECTION
#endif

  }; /* CustomHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
  {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
  {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'D', 0, 'a', 0, 'J', 0, 'i', 0, 'a', 0, 'o', 0, 'Y', 0, 'a', 0,
    'e', 0, 'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0,
    'i', 0, 'c', 0
  };

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
  {
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'D', 0, 'a', 0, 'J', 0, 'i', 0, 'a', 0, 'o', 0, 'C', 0,
    'u', 0, 's', 0, 't', 0, 'm', 0, ' ', 0, 'H', 0, 'I', 0,
    'D', 0
  };
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
  {
    CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'D', 0, 'J', 0, 'Y', 0,'6', 0,'8', 0, '1', 0, '1', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

