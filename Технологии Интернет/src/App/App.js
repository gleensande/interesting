import React, { PureComponent, Fragment } from 'react';
import Stage from '../Stage/Stage.js';
import Button from '../Button/Button.js';
import Header from '../Header/Header.js';

class App extends PureComponent {
	constructor(props) {
		super(props);
		this.state = {
			stages : [],
			newAppendedTask : '',
			newIndexOfAppend : 0,
		};

		this.handleClickOnAddStageButton = this.handleClickOnAddStageButton.bind(this);
		this.handleMoveTask = this.handleMoveTask.bind(this);
	}

	handleClickOnAddStageButton() {
		this.setState(state => {
			const stagesAdd = state.stages.concat('1');

			return {
				...state,
				stages : stagesAdd,
			};
		});
	}

	handleMoveTask(from, taskInnerText) {
		console.log("from:", from);
		console.log("taskInnerText:", taskInnerText);

		if (from + 1 >= this.state.stages.length) {
			alert(":) Задача удалена (:");
			return;
		}

		this.setState(state => ({
			...state,
			newAppendedTask : taskInnerText,
			newIndexOfAppend : from + 1,
		}));
	}

	render() {
		return (
			<Fragment>
				<div id = "application">
					<Header 
						className = "workflow_header"
						placeholder = "Название потока"		
					/>
					<div className = "workflow">
						{this.state.stages.map((_, index) => 
							<Stage 
								key = {index}
								id = {index}
								handleMoveTask = {this.handleMoveTask}
								newAppendedTask = {this.state.newAppendedTask}
								newIndexOfAppend = {this.state.newIndexOfAppend}
							/>
						)} 
						<Button 
							className = "add_stage_button"
							innerText = "+ добавить стадию"
							handleClick = {this.handleClickOnAddStageButton}
						/>
					</div>
				</div>
			</Fragment>
		);
	}
}

export default App;
