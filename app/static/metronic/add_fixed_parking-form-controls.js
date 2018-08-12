//== Class definition

var FormControls = function () {
    //== Private functions

    var demo1 = function () {
        $("#m_form_1").validate({
            // define validation rules
            rules: {
                new_plate_number: {
                    required: true
                },
                new_space_number: {
                    required: false,
                },
                new_company_name: {
                    required: true
                },
                dateTimeRange2: {
                    required: true,
                }
            },

            //display error alert on form submit  
            invalidHandler: function (event, validator) {
                var alert = $('#m_form_1_msg');
                alert.removeClass('m--hide').show();
                mApp.scrollTo(alert, -200);
            },

            submitHandler: function (form) {
                var this_modal = $('#add_fixed_parking_record');
                var new_plate_number = $('#new_plate_number').val();
                var new_space_number = $('#new_space_number').val();
                var new_company_name = $('#new_company_name').val();
                var new_validate_period = $('#dateTimeRange2').val();

                var params = {
                    'new_plate_number': new_plate_number,
                    'new_space_number': new_space_number,
                    'new_company_name': new_company_name,
                    'new_validate_period': new_validate_period
                };
                $.ajax({
                    type: "POST",          //提交方式          
                    url: "add_fixed_parking_record",  //提交的页面/方法名          
                    dataType: 'json',
                    contentType: "application/json; charset=utf-8",
                    data: JSON.stringify(params, null, '\t'),         //参数（如果没有参数：null）          

                    success: function (msg) {
                        if (msg.status === 'true') {
                            $('#ajax_data').mDatatable().destroy();
                            DatatableRemoteAjaxDemo.init();
                            toastr.info(msg.content);
                            $('#add_fixed_parking_record').modal('hide')
                        }
                        else {
                            toastr.error(msg.content);
                            $('#add_fixed_parking_record').modal('hide')
                        }
                    },
                    error: function (xhr, msg, e) {
                        toastr.error(msg.content);
                        $('#add_fixed_parking_record').modal('hide')
                    }
                });
            }
        });
    }

    var demo2 = function () {
        $("#m_form_update").validate({
            // define validation rules
            rules: {
                update_plate_number: {
                    required: false
                },
                update_space_number: {
                    required: false,
                },
                update_company_name: {
                    required: false
                },
                update_dateTimeRange: {
                    required: false,
                }
            },

            //display error alert on form submit
            invalidHandler: function (event, validator) {
                var alert = $('#m_form_update_msg');
                alert.removeClass('m--hide').show();
                mApp.scrollTo(alert, -200);
            },

            submitHandler: function (form) {
                let uuid = $('#record_uuid').val();
                let update_plate_number = $('#update_plate_number').val();
                let update_space_number = $('#update_space_number').val();
                let update_company_name = $('#update_company_name').val();
                let update_dateTimeRange = $('#update_dateTimeRange').val();

                let params = {
                    'uuid': uuid,
                    'update_plate_number': update_plate_number,
                    'update_space_number': update_space_number,
                    'update_company_name': update_company_name,
                    'update_dateTimeRange': update_dateTimeRange
                };
                $.ajax({
                    type: "POST",          //提交方式          
                    url: "fixed_parking_record_update",  //提交的页面/方法名          
                    dataType: 'json',
                    contentType: "application/json; charset=utf-8",
                    data: JSON.stringify(params, null, '\t'),         //参数（如果没有参数：null）          

                    success: function (msg) {
                        if (msg.status === 'true') {
                            $('#ajax_data').mDatatable().destroy();
                            DatatableRemoteAjaxDemo.init();
                            toastr.info(msg.content);
                            $('#update_fixed_parking_record').modal('hide')
                        }
                        else {
                            toastr.warn(msg.content);
                            $('#update_fixed_parking_record').modal('hide')
                        }
                    },
                    error: function (xhr, msg, e) {
                        toastr.error(msg.content);
                        $('#update_fixed_parking_record').modal('hide')
                    }
                });
            }
        });
    }


    return {
        // public functions
        init: function () {
            demo1();
            demo2();
        }
    };
}();

jQuery(document).ready(function () {
    FormControls.init();
});