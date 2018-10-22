//== Class definition

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
                        url: '/fixed_parking_space',
                        params: {
                            // custom query params
                            query: {
                                company_name: $('#company_name').val(),
                                plate_or_space_number: $('#plate_or_space_number').val(),
                                search_date: $('#fixed_daterange_search .form-control').val(),
                            }
                        },
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
                    field: 'number_plate',
                    textAlign: 'center',
                    title: '车牌号',
                }, {
                    field: 'specified_parking_space_code',
                    textAlign: 'center',
                    title: '车位编号',
                }, {
                    field: 'company',
                    textAlign: 'center',
                    title: '公司名称',
                }, {
                    field: 'order_validate_start',
                    textAlign: 'center',
                    title: '开始时间',
                    type: 'date',
                    template: function(data, type, full, meta) {
						//时间格式化
                        if (data.order_validate_start) {
                            return  moment(data.order_validate_start).utc().format("YYYY-MM-DD HH:mm:ss");
                        }
						else {
                            return ''
                        }
					}
                }, {
                    field: 'order_validate_stop',
                    textAlign: 'center',
                    title: '结束时间',
                    type: 'date',
                    template: function(data, type, full, meta) {
						//时间格式化
                        if (data.order_validate_stop) {
                            return  moment(data.order_validate_stop).utc().format("YYYY-MM-DD HH:mm:ss");
                        }
						else {
                            return ''
                        }
					}
                }, {
                    field: 'Actions',
                    //width: 110,
                    textAlign: 'center',
                    title: '操作',
                    sortable: false,
                    overflow: 'visible',
                    width: 40,
                    template: function (row, index, datatable) {
                        return '<a data-toggle="modal" data-target="#update_fixed_parking_record" onclick="editInfo(\'' + row.id + '\', \'' + row.number_plate + '\', \'' + row.specified_parking_space_code + '\', \'' + row.company + '\')" class="m-portlet__nav-link btn m-btn m-btn--hover-accent m-btn--icon m-btn--icon-only m-btn--pill" title="Edit details">\
                                    <i class="la la-edit"></i>\
                                </a>\
                                <a ' + 'onClick="return HTMerDel(\'' + row.id + '\')" class="m-portlet__nav-link btn m-btn m-btn--hover-danger m-btn--icon m-btn--icon-only m-btn--pill" title="Delete">\
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
});

$('#submit_search').click(function () {
    $('#ajax_data').mDatatable().destroy();
    DatatableRemoteAjaxDemo.init();
})