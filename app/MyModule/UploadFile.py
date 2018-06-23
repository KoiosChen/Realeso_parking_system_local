import os


class uploadfile():
    def __init__(self, name, type=None, size=None, not_allowed_msg='', service_url='', order_status=''):
        self.name = name
        self.type = type
        self.size = size
        self.not_allowed_msg = not_allowed_msg
        self.url = "data" + service_url + "/%s" % name
        self.import_url = "import_duty" + service_url + "/%s" % name
        self.thumbnail_url = "thumbnail" + service_url + "/%s" % name
        self.delete_url = "delete" + service_url + "/%s" % name
        self.delete_type = "DELETE"
        self.id = name.replace('.', '')
        self.order_status = order_status

    def is_image(self):
        fileName, fileExtension = os.path.splitext(self.name.lower())

        if fileExtension in ['.jpg', '.png', '.jpeg', '.bmp', '.pcap', '.pcapng']:
            return True

        return False

    def pcap_finished(self):
        pass

    def get_file(self):
        if self.type is not None:
            # POST an image
            if self.type.startswith('image'):
                return {"name": self.name,
                        "type": self.type,
                        "size": self.size, 
                        "url": self.url,
                        "id": self.id,
                        "thumbnailUrl": self.thumbnail_url,
                        "deleteUrl": self.delete_url, 
                        "deleteType": self.delete_type,}
            
            # POST an normal file
            elif self.not_allowed_msg == '':
                return {"name": self.name,
                        "type": self.type,
                        "size": self.size, 
                        "url": self.url,
                        "id": self.id,
                        "deleteUrl": self.delete_url,
                        "importUrl": self.import_url,
                        "order_status": self.order_status,
                        "deleteType": self.delete_type,}

            # File type is not allowed
            else:
                return {"error": self.not_allowed_msg,
                        "name": self.name,
                        "type": self.type,
                        "size": self.size,}

        # GET image from disk
        elif self.is_image():
            return {"name": self.name,
                    "size": self.size, 
                    "url": self.url,
                    "id": self.id,
                    "thumbnailUrl": self.thumbnail_url,
                    "deleteUrl": self.delete_url, 
                    "deleteType": self.delete_type,}
        
        # GET normal file from disk
        else:
            return {"name": self.name,
                    "size": self.size, 
                    "url": self.url,
                    "id": self.id,
                    "deleteUrl": self.delete_url,
                    "importUrl": self.import_url,
                    "order_status": self.order_status,
                    "deleteType": self.delete_type,}
