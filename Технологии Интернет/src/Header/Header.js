import React, { PureComponent, Fragment } from 'react';

class Header extends PureComponent {
	constructor(props) {
		super(props);
		this.state = {
			value: '', 
			className: props.className,
			placeholder : props.placeholder,
			handleHeaderValueChange : props.handleHeaderValueChange,
		};

		this.handleChange = this.handleChange.bind(this);
	}

	handleChange(event) {
		this.setState({value: event.target.value});
		if (this.state.handleHeaderValueChange) {
			this.state.handleHeaderValueChange(event.target.value);
		}
	}

	render() {
		return (
			<Fragment>
			<input 
				type = "text" 
				className = {this.state.className} 
				value = {this.state.value} 
				onChange = {this.handleChange} 
				placeholder = {this.state.placeholder}
				maxLength = "15"
			/>
			</Fragment>
		);
	}
}

export default Header;
