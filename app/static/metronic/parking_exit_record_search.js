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
                        url: '/parking_exit_record',
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
                    field: 'camera',
                    title: '摄像头编号',
                    sortable: false, // disable sort for this column
                    selector: false,
                    textAlign: 'center',
                }, {
                    field: 'number_plate_pic',
                    title: '入场照片',
                    width: 180,
                    textAlign: 'center',
                    template: function (data) {
                        let http_img = "http://" + document.domain + data.exit_pic;

                        output = '<div class="m-card-user m-card-user--lg">\
								<div class="m-card-user__pic">\
									<img src=' + http_img + ' class="m--marginless" alt="photo">\
								</div>\
								<div class="m-card-user__details">\
									<span class="m-card-user__name">' + data.number_plate + '</span>\
								</div>\
							</div>';


                        return output;
                    },
                }, {
                    field: 'exit_time',
                    textAlign: 'center',
                    title: '出场时间',
                    type: 'date',
                    format: 'MM/DD/YYYY',
                }, {
                    field: 'Actions',
                    //width: 110,
                    textAlign: 'center',
                    title: '操作',
                    sortable: false,
                    overflow: 'visible',
                    width: 40,
                    template: function (row, index, datatable) {
                        return '<a data-toggle="modal" data-target="#update" onclick="editInfo(\'' + row.camera + '\', \''  + row.number_plate + '\')" class="m-portlet__nav-link btn m-btn m-btn--hover-accent m-btn--icon m-btn--icon-only m-btn--pill" title="Edit details">\
                                    <i class="la la-edit"></i>\
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