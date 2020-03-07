import React, { PureComponent, Fragment } from 'react';
import Task from '../Task/Task.js';
import Button  from '../Button/Button.js';
import Header from '../Header/Header.js';

class Stage extends PureComponent {
	constructor(props) {
		super(props);
		this.state = {
			id : props.id,
			handleMoveTask : props.handleMoveTask,
			newAppendedTask: props.newAppendedTask,
			newIndexOfAppend: props.newIndexOfAppend,
			headerValue : '',
			tasksInnerText : [],
		};

		this.handleHeaderValueChange = this.handleHeaderValueChange.bind(this);
		this.handleClickOnAddTaskButton = this.handleClickOnAddTaskButton.bind(this);
		this.handleClickOnMoveTaskButton = this.handleClickOnMoveTaskButton.bind(this);
	}

	componentDidUpdate(prevProps) {
		// если добавляют по моему индексу и (индекс или задача) поменялись с прошлого раза
		if (			
			(this.props.newIndexOfAppend === this.props.id &&
			this.props.newIndexOfAppend !== prevProps.newIndexOfAppend) ||
			(this.props.newIndexOfAppend === this.props.id &&
			this.props.newAppendedTask !== prevProps.newAppendedTask)	
		) {
			this.setState(state => {
				const tasks_add = state.tasksInnerText.concat(this.props.newAppendedTask);
				
				return {
					...state,
					tasksInnerText : tasks_add,
				};
			});	
		}
	}

	handleHeaderValueChange(newHeaderValue) {
		this.setState({headerValue: newHeaderValue});
	}

	handleClickOnAddTaskButton() {
		const nextTaskInnerText = prompt("Введите задачу:");
		if (!nextTaskInnerText) {
			return;
		}

		this.setState(state => {
			const tasks_add = state.tasksInnerText.concat(nextTaskInnerText);
			
			return {
				...state,
				headerValue : state.headerValue,
				tasksInnerText : tasks_add,
			};
		});
	}

	handleClickOnMoveTaskButton() {
		if (this.state.tasksInnerText.length === 0) {
			return;
		}

		// запоминаем переносимую задачу
		const taskInnerText = this.state.tasksInnerText[this.state.tasksInnerText.length - 1];
		const id = this.state.id;

		// удаляем задачу из текущей стадии
		this.setState(state => {
			return {
				...state,
				headerValue : state.headerValue,
				tasksInnerText : this.state.tasksInnerText.filter((_, i) => 
					i !== (this.state.tasksInnerText.length - 1)
				),
			};
		});

		// отправляем данные в компонент Приложение
		this.state.handleMoveTask(id, taskInnerText);
	}


	render() {
		let computedInnerText = "+ добавить задачу";
		if (this.state.headerValue !== '') {
			computedInnerText += " к '" + this.state.headerValue + "'";
		}
		return (
			<Fragment>
				<div className = "stage">
					<Header 
						className = "tasks_header"
						placeholder = "Название стадии"
						handleHeaderValueChange = {this.handleHeaderValueChange}
					/>
					<div className = "tasks">
						<Button 
							className = "add_task_button"
							innerText = {computedInnerText}
							handleClick = {this.handleClickOnAddTaskButton}
						/>
						
						{this.state.tasksInnerText.map((taskInnerText, index) => 
							<Task
								key = {index}
								innerText = {taskInnerText}
							/>
						)}
												
						<Button 
							className = "add_task_button"
							innerText = "→ переместить последнюю"
							handleClick = {this.handleClickOnMoveTaskButton}
						/>
					</div>
				</div>
			</Fragment>
		);
	}
}

export default Stage;
