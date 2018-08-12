//== Class definition

var BootstrapDaterangepicker = function () {
    
    //== Private functions
    var demos = function () {
        // minimum setup

        // date & time
        $('#m_daterangepicker_4').daterangepicker({
            buttonClasses: 'm-btn btn',
            applyClass: 'btn-primary',
            cancelClass: 'btn-secondary',

            timePicker: true,
            timePickerIncrement: 1,
            timePicker24Hour: true,
            timePickerSeconds: true,
            locale: {
                format: 'MM/DD/YYYY h:mm A'
            }
        }, function(start, end, label) {
            $('#m_daterangepicker_4 .form-control').val( start.format('YYYY/MM/DD h:mm:ss') + ' - ' + end.format('YYYY/MM/DD h:mm:ss'));
        });

        $('#fixed_daterange_search').daterangepicker({
            buttonClasses: 'm-btn btn',
            applyClass: 'btn-primary',
            cancelClass: 'btn-secondary',

            timePicker: false,
            timePickerIncrement: 1,
            timePicker24Hour: true,
            timePickerSeconds: true,
            locale: {
                format: 'MM/DD/YYYY 00:00:00'
            }
        }, function(start, end, label) {
            $('#fixed_daterange_search .form-control').val( start.format('YYYY/MM/DD 00:00:00') + ' - ' + end.format('YYYY/MM/DD 00:00:00'));
        });

        $('#m_daterangepicker_5').daterangepicker({
            buttonClasses: 'm-btn btn',
            applyClass: 'btn-primary',
            cancelClass: 'btn-secondary',

            timePicker: false,
            timePickerIncrement: 1,
            timePicker24Hour: true,
            timePickerSeconds: true,
            locale: {
                format: 'MM/DD/YYYY 00:00:00'
            }
        }, function(start, end, label) {
            $('#m_daterangepicker_5 .form-control').val( start.format('YYYY/MM/DD 00:00:00') + ' - ' + end.format('YYYY/MM/DD 00:00:00'));
        });

        $('#m_daterangepicker_6').daterangepicker({
            buttonClasses: 'm-btn btn',
            applyClass: 'btn-primary',
            cancelClass: 'btn-secondary',

            timePicker: false,
            timePickerIncrement: 1,
            timePicker24Hour: true,
            timePickerSeconds: true,
            locale: {
                format: 'MM/DD/YYYY 00:00:00'
            }
        }, function(start, end, label) {
            $('#m_daterangepicker_6 .form-control').val( start.format('YYYY/MM/DD 00:00:00') + ' - ' + end.format('YYYY/MM/DD 00:00:00'));
        });

    }


    return {
        // public functions
        init: function() {
            demos(); 
        }
    };
}();

jQuery(document).ready(function() {    
    BootstrapDaterangepicker.init();
});