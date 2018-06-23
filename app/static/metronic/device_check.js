//== Class definition

var FormControls = function () {
    //== Private functions

    var demo1 = function () {
        $("#m_form_1").validate({
            // define validation rules
            rules: {
                device_name: {
                    required: true
                },
                device_ip: {
                    ipv4: true,
                    required: true,
                },
                machine_room: {
                    required: true
                }
            },

            //display error alert on form submit
            invalidHandler: function (event, validator) {
                var alert = $('#m_form_1_msg');
                alert.removeClass('m--hide').show();
                mApp.scrollTo(alert, -200);
            },

            submitHandler: function (form) {
                var device_name = $('#device_name').val();
                var device_ip = $('#device_ip').val();
                var machine_room = $('#machine_room').val();

                var params = {
                    'device_name': device_name,
                    'device_ip': device_ip,
                    'machine_room': machine_room
                };
                $.ajax({
                    type: "POST",          //提交方式          
                    url: "device_add",  //提交的页面/方法名          
                    dataType: 'json',
                    contentType: "application/json; charset=utf-8",
                    data: JSON.stringify(params, null, '\t'),         //参数（如果没有参数：null）          

                    success: function (msg) {
                        if (msg.status === 'OK') {
                            toastr.info(msg.content);
                            $('#ajax_data').mDatatable().destroy();
                            DatatableRemoteAjaxDemo.init();
                            $('#add_device').modal('hide')
                        }
                        else {
                            toastr.warning(msg.content);
                            $('#add_device').modal('hide')
                        }
                    },
                    error: function (xhr, msg, e) {
                        toastr.warning(msg.content);
                        $('#add_device').modal('hide')
                    }
                });
            }
        });
    }



    return {
        // public functions
        init: function () {
            demo1();
        }
    };
}();


var DatatableRemoteAjaxDemo = function () {
    //== Private functions

    // basic demo
    var demo = function () {

        var datatable = $('#ajax_data').mDatatable({
            // datasource definition
            data: {
                type: 'remote',
                source: {
                    read: {
                        // sample GET method
                        method: 'POST',
                        url: '/devices_manage',
                        map: function (raw) {
                            // sample data mapping
                            var dataSet = raw;
                            if (typeof raw.data !== 'undefined') {
                                dataSet = raw.data;
                            }
                            return dataSet;
                        },
                    },
                },
                pageSize: 10,
                serverPaging: true,
                serverFiltering: true,
                serverSorting: false,
            },

            // layout definition
            layout: {
                scroll: false,
                footer: false
            },

            // column sorting
            sortable: false,

            pagination: true,

            autoWidth: true,

            toolbar: {
                // toolbar items
                items: {
                    // pagination
                    pagination: {
                        // page size select
                        pageSizeSelect: [10, 20, 30, 50, 100],
                    },
                },
            },

            // columns definition

            columns: [
                {
                    field: 'id',
                    title: '#',
                    sortable: false, // disable sort for this column
                    selector: false,
                    textAlign: 'center',
                }, {
                    field: 'device_name',
                    title: '主机名',
                    textAlign: 'center',
                    // sortable: 'asc', // default sort
                    filterable: false, // disable or enable filtering
                    // basic templating support for column rendering,
                }, {
                    field: 'device_ip',
                    title: '管理IP',
                    textAlign: 'center',
                }, {
                    field: 'machine_room',
                    title: '所属机房',
                    textAlign: 'center',
                }, {
                    field: 'device_status',
                    title: '设备状态',
                    textAlign: 'center',
                }, {
                    field: 'Actions',
                    textAlign: 'center',
                    title: '操作',
                    sortable: false,
                    overflow: 'visible',
                    template: function (row, index, datatable) {
                        return '<a ' + 'onClick="return HTMerDel(' + row.id + ')" class="m-portlet__nav-link btn m-btn m-btn--hover-danger m-btn--icon m-btn--icon-only m-btn--pill" title="Delete">\
                                    <i class="la la-trash"></i>\
                                </a>\
                            ';
                    },
                }],
        });
    };



    return {
        // public functions
        init: function () {
            demo();
        },
    };
}();

jQuery(document).ready(function () {
    DatatableRemoteAjaxDemo.init();
    FormControls.init()
});

$('#submit_search').click(function () {
    $('#ajax_data').mDatatable().destroy();
    DatatableRemoteAjaxDemo.init();
})