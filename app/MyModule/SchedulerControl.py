from .. import logger, scheduler
from .GetConfig import get_config


def scheduler_resume():
    scheduler_config = get_config('scheduler')
    for key in scheduler_config.keys():
        try:
            logger.warn('Resume {}'.format(key))
            scheduler.resume_job(id=key)
        except Exception as e:
            logger.warn({'status': '%s resume fail' % key})


def scheduler_pause():
    scheduler_config = get_config('scheduler')
    for key in scheduler_config.keys():
        try:
            logger.warn('Pausing {}'.format(key))
            scheduler.pause_job(id=key)
        except Exception as e:
            logger.warn({'status': '%s pasue fail' % key})


def scheduler_modify():
    # 获取计划任务参数
    scheduler_config = get_config('scheduler')
    logger.info(scheduler_config)
    for key, value in scheduler_config.items():
        interval = float(value.strip())
        scheduler_id = key
        try:
            if interval:
                scheduler.pause_job(id=scheduler_id)
                scheduler.modify_job(id=scheduler_id, trigger='interval', seconds=interval)
                scheduler.resume_job(id=scheduler_id)
            else:
                scheduler.pause_job(id=scheduler_id)
        except Exception as e:
            logger.warn({'status': '%s 提交计划任务失败' % scheduler_id})
