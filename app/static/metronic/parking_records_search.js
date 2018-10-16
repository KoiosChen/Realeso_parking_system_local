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
                        url: '/parking_records',
                        params: {
                            // custom query params
                            query: {
                                record_status: $('#record_status').val(),
                                search_content: $('#search_content').val(),
                                search_date: $('#m_daterangepicker_4 .form-control').val(),
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
                    field: 'number_plate_pic',
                    title: '入场照片',
                    width: 180,
                    textAlign: 'center',
                    template: function (data) {
                        let user_img = data.entry_plate_number_pic;
                        http_img = "http://" + document.domain + "/" + user_img

                        output = '<div class="m-card-user m-card-user--lg">\
								<div class="m-card-user__pic">\
									<img src=' + user_img + ' class="m--marginless" alt="photo">\
								</div>\
								<div class="m-card-user__details">\
									<span class="m-card-user__name">' + data.number_plate + '</span>\
								</div>\
							</div>';


                        return output;
                    },
                }, {
                    field: 'entry_time',
                    textAlign: 'center',
                    title: '入场时间',
                    overflow: 'visible',
                    type: 'date',
                    template: function(data, type, full, meta) {
						//时间格式化
						return  moment(data.entry_time).utc().format("YYYY-MM-DD HH:mm:ss");
					}

                }, {
                    field: 'entry_gate',
                    textAlign: 'center',
                    title: '入场闸机',
                }, {
                    field: 'exit_time',
                    textAlign: 'center',
                    title: '出场时间',
                    type: 'date',
                    template: function(data, type, full, meta) {
						//时间格式化
                        if (data.exit_time) {
                            return  moment(data.exit_time).utc().format("YYYY-MM-DD HH:mm:ss");
                        }
						else {
                            return ''
                        }
					}
                }, {
                    field: 'exit_gate',
                    textAlign: 'center',
                    title: '出场闸机',
                }, {
                    field: 'status',
                    textAlign: 'center',
                    title: '状态',
                }, {
                    field: 'Actions',
                    //width: 110,
                    textAlign: 'center',
                    title: '操作',
                    sortable: false,
                    overflow: 'visible',
                    width: 40,
                    template: function (row, index, datatable) {
                        return '<a data-toggle="modal" data-target="#update" onclick="editInfo(\'' + row.id + '\', \''  + row.number_plate + '\')" class="m-portlet__nav-link btn m-btn m-btn--hover-accent m-btn--icon m-btn--icon-only m-btn--pill" title="Edit details">\
                                    <i class="la la-edit"></i>\
                                </a>\
                                <a onclick="center_pay_info(\'' + row.id + '\', \''  + row.number_plate + '\')" class="m-portlet__nav-link btn m-btn m-btn--hover-accent m-btn--icon m-btn--icon-only m-btn--pill" title="Edit details">\
                                    <i class="la la-cny"></i>\
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